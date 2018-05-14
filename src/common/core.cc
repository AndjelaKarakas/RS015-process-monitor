#include "core.h"

#include <sys/types.h>
#include <sys/sysinfo.h>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <map>
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

ProcessInfo Core::get_pid_info(int pid) {
  ProcessInfo ret;
  std::ifstream infile("/proc/" + std::to_string(pid) + "/stat");
  std::string temp;
  
  infile >> ret.pid;
  infile >> ret.name;
  
  // TODO: Use some of this info later... also optimize?
  infile >> temp;
  infile >> temp;
  infile >> temp;
  infile >> temp;
  infile >> temp;
  infile >> temp;
  infile >> temp;
  infile >> temp;
  infile >> temp;
  infile >> temp;
  infile >> temp;
  infile >> temp;
  infile >> temp;
  infile >> temp;
  infile >> temp;
  infile >> temp;
  infile >> temp;
  infile >> temp;
  infile >> temp;
  infile >> temp;

  infile >> ret.memory;

  infile.close();
  return ret;
}

void Core::refresh_pids() {
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

    if (curnum != -1)
      pids_.push_back(curnum);
  }
}

SysMemoryInfo& Core::get_memory_usage() {
  return mem_info_;
}

std::map<std::string, DiskInfo>& Core::get_disk_usage_info() {
  return disk_info_;
}

void Core::refresh_resources() {
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
    valid = (name.find("sd") == 0);

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
}

}