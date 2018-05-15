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

  double get_cpu_percentage_usage();
  void update(long cpu_jits);

  // Name of the process.
  std::string name;

  // Process id number.
  int pid;

  // Memory used (in bytes).
  long memory;

  // Process priority, goes from 0 to 139.
  int priority;

  // Nice level, goes from -20 to 19.
  int nice_level;

 private:
  long total_cpu_jits_;
  long cpu_jits_;
};

}

#endif