#ifndef PROCESSMONITOR_COMMON_PROCESSINFO_H_
#define PROCESSMONITOR_COMMON_PROCESSINFO_H_

#include <string>
#include <map>

#include "cpuinfo.h"

namespace ProcessMonitor {

// Contains the basic information about a process.
class ProcessInfo {
 public:
  ProcessInfo();

  long get_bytes_read();
  long get_bytes_written();
  double get_cpu_percentage_usage();
  void update(long cpu_jits, long bytes_read, long bytes_written);

  // Name of the process.
  std::string name;

  // Process id number.
  int pid;

  // Process uid number.
  int uid;

  // Memory used (in bytes).
  long memory;

  // Process priority, goes from 0 to 139.
  int priority;

  // Nice level, goes from -20 to 19.
  int nice_level;

 private:
  long total_cpu_jits_;
  long cpu_jits_;
  long total_bytes_read_;
  long bytes_read_;
  long total_bytes_written_;
  long bytes_written_;
};

}

#endif