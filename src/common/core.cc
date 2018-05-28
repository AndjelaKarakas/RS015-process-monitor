#include "core.h"

#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <map>
#include <unistd.h>
#include <vector>
#include <stdexcept>
#include <string>

namespace ProcessMonitor {

Core::Core() :
  steam_diskstats_("/proc/diskstats") {

  proc_dir_ = opendir("/proc");
  if (proc_dir_ == NULL)
    throw std::runtime_error("Failed to open /proc directory.");
}

Core& Core::getInstance() {
  static Core instance;
  return instance;
}

std::vector<int>& Core::get_pid_list() {
  return pids_;
}

bool Core::valid_pid(int pid) {
  std::ifstream infile("/proc/" + std::to_string(pid) + "/stat");
  return infile.good();
}

ProcessInfo& Core::get_pid_info(int pid) {
  return pid_info_[pid];
}

SysMemoryInfo& Core::get_memory_usage() {
  return mem_info_;
}

std::map<std::string, DiskInfo>& Core::get_disk_usage_info() {
  return disk_info_;
}

std::map<std::string, CpuInfo>& Core::get_cpu_usage_info() {
  return cpu_info_;
}

NetInfo& Core::get_network_info() {
  return net_info_;
}

void Core::refresh() {
  // Poll Processes
  pids_.clear();

  struct dirent* entry;
  int curnum;

  seekdir(proc_dir_, 0);
  while ((entry = readdir(proc_dir_)) != NULL) {
    curnum = 0;

    for (int i = 0; i < 256; i++) {
      if (isdigit(entry->d_name[i])) {
        // TODO: optimize?
        curnum = curnum * 10 + (entry->d_name[i] - 48);
      } else {
        if (entry->d_name[i] != '\0')
          curnum = -1;

        break;
      }
    }

    if (curnum != -1) {
      if (stat(("/proc/" + std::to_string(curnum) + "/exe").c_str(), &statbuf_) != 0)
        continue;
        
      pids_.push_back(curnum);

      auto pid = curnum;
      auto elem = pid_info_.find(pid);

      // update pid info
      long ujifs, kjifs;
      long bytes_read = 0;
      long bytes_written = 0;
      std::string temp;

      if (elem == pid_info_.end())
      {
        pid_info_[pid] = ProcessInfo();
        elem = pid_info_.find(pid);
      }

      if (stat(("/proc/" + std::to_string(pid)).c_str(), &statbuf_) == 0)
        elem->second.uid = statbuf_.st_uid;

      auto& iofile = elem->second.iostream;
      if (!iofile.is_open())
        iofile.open("/proc/" + std::to_string(pid) + "/io");
      iofile.seekg(0, std::ios::beg);
      while (iofile.peek() != EOF) {
        iofile >> temp;
        if (temp == "read_bytes:")
          iofile >> bytes_read;
        else if (temp == "write_bytes:")
          iofile >> bytes_written;
        else
          iofile.ignore(255, '\n');
      }
      iofile.clear();

      auto& infile = elem->second.statstream;
      if (!infile.is_open())
        infile.open("/proc/" + std::to_string(pid) + "/stat");
      infile.seekg(0, std::ios::beg);
      
      infile >> elem->second.pid;         // process id
      infile >> elem->second.name;        // filename of the executable
      infile.ignore(1);
      
      infile.ignore(255, ' ');            // state (R is running, S is sleeping, D is sleeping in an uninterruptible wait, Z is zombie, T is traced or stopped)
      infile.ignore(255, ' ');            // process id of the parent process
      infile.ignore(255, ' ');            // pgrp of the process
      infile.ignore(255, ' ');            // session id
      infile.ignore(255, ' ');            // tty the process uses
      infile.ignore(255, ' ');            // pgrp of the tty
      infile.ignore(255, ' ');            // task flags
      infile.ignore(255, ' ');            // number of minor faults
      infile.ignore(255, ' ');            // number of minor faults with child's
      infile.ignore(255, ' ');            // number of major faults
      infile.ignore(255, ' ');            // number of major faults with child's
      
      infile >> ujifs;                    // user mode jiffies
      infile >> kjifs;                    // user mode jiffies
      infile.ignore(1);
      
      infile.ignore(255, ' ');            // user mode jiffies with child's
      infile.ignore(255, ' ');            // kernel mode jiffies with child's

      infile >> elem->second.priority;    // priority level
      infile >> elem->second.nice_level;  // nice level
      infile.clear();

      // Get memory info
      if (elem->second.total_cpu_jits > ujifs + kjifs || elem->second.total_cpu_jits == -1) {
        auto& smapsfile = elem->second.mapstream;
        if (!smapsfile.is_open())
          smapsfile.open("/proc/" + std::to_string(pid) + "/smaps");
        smapsfile.seekg(0, std::ios::beg);
        bool readme = false;
        int extramemory;

        elem->second.memory = 0;
        while (smapsfile.peek() != EOF) { // Dear god, this is bad, OPTIMIZE!!!
          if (readme) {
            smapsfile >> extramemory;
            elem->second.memory += extramemory;
            readme = false;

            if (extramemory > 0) {
              readme = false;
            }
          } else {
            smapsfile >> temp;
            if (temp == "Private_Dirty:")
              readme = true;
          }
        }
        smapsfile.clear();
        elem->second.memory *= 1024;
      }

      elem->second.update(ujifs + kjifs, bytes_read, bytes_written);
    }
  }

  // Poll RAM
  if (sysinfo(&sinfo_) == 0) {
    mem_info_.memory_usage = sinfo_.totalram - sinfo_.freeram;
    mem_info_.swap_usage = sinfo_.totalswap - sinfo_.freeswap;
    mem_info_.total_memory = sinfo_.totalram;
    mem_info_.total_swap = sinfo_.totalswap;
  }

  // Poll Drives
  std::string name;
  long reads, writes;
  bool valid = false;

  steam_diskstats_.clear();
  steam_diskstats_.seekg(0, std::ios::beg);

  while (steam_diskstats_.peek() != EOF) { // TODO: Remove disconnected drives.
    steam_diskstats_.ignore(4);              // major number
    steam_diskstats_.ignore(8);              // minnor number

    steam_diskstats_ >> name;                // device name
    steam_diskstats_.ignore(1);
    valid = valid_drive_name(name);

    steam_diskstats_.ignore(255, ' ');       // reads completed successfully
    steam_diskstats_.ignore(255, ' ');       // reads merged

    if (valid) {                             // sectors read
      steam_diskstats_ >> reads;
      steam_diskstats_.ignore(1);
    } else {
      steam_diskstats_.ignore(255, ' ');
    }

    steam_diskstats_.ignore(255, ' ');       // time spent reading (ms)
    steam_diskstats_.ignore(255, ' ');       // writes completed
    steam_diskstats_.ignore(255, ' ');       // writes merged

    if (valid) {                             // sectors written
      steam_diskstats_ >> writes;
      steam_diskstats_.ignore(1);
    } else {
      steam_diskstats_.ignore(255, ' ');
    }

    steam_diskstats_.ignore(255, ' ');       // time spent writing (ms)
    steam_diskstats_.ignore(255, ' ');       // I/Os currently in progress
    steam_diskstats_.ignore(255, ' ');       // time spent doing I/Os (ms)
    steam_diskstats_.ignore(255, '\n');      // weighted time spent doing I/Os (ms)

    if (valid) {
      auto elem = disk_info_.find(name);

      if (elem == disk_info_.end())
        disk_info_[name] = DiskInfo(reads, writes);
      else
        elem->second.update(reads, writes);
    }
  }

  // Poll CPU
  std::ifstream cpustream("/proc/stat");
  long value, used, freed;

  while (cpustream.peek() != EOF) {
    cpustream >> name;  // CPU name / umber

    if (name.find("cpu") != 0)
      break;

    used = 0;
    freed = 0;

    cpustream >> value; // user: normal processes executing in user mode
    used += value;
    cpustream >> value; // nice: niced processes executing in user mode
    used += value;
    cpustream >> value; // system: processes executing in kernel mode
    used += value;
    cpustream >> value; // idle: twiddling thumbs
    freed += value;
    cpustream >> value; // iowait: In a word, iowait stands for waiting for I/O to complete.
    freed += value;
    cpustream >> value; // irq: servicing interrupts
    freed += value;
    cpustream >> value; // softirq: servicing softirqs
    used += value;
    cpustream >> value; // steal: involuntary wait
    freed += value;
    cpustream >> value; // guest: running a normal guest
    used += value;
    cpustream >> value; // guest_nice: running a niced guest
    used += value;

    auto elem = cpu_info_.find(name);
    if (elem == cpu_info_.end())
      cpu_info_[name] = CpuInfo(used, freed);
    else
      elem->second.update(used, freed);
  }

  // Poll Network
  net_info_.update();
}

bool Core::valid_drive_name(std::string name) {
  for (auto& c : name)
    if (isdigit(c))
      return false;

  return (name.find("sd") == 0);
}

}