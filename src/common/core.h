#ifndef PROCESSMONITOR_COMMON_CORE_H_
#define PROCESSMONITOR_COMMON_CORE_H_

#include <sys/sysinfo.h>
#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include <map>
#include <vector>

#include "diskinfo.h"
#include "processinfo.h"
#include "sysmemoryinfo.h"

namespace ProcessMonitor {

// Used to get system information.
// Use getInstance() to use it.
//
// Example:
//    Core::getInstance().refresh_pids();
class Core {
 public:
  Core(Core const&) = delete;
  void operator=(Core const&) = delete;

  // Returns an instance of the Core class.
  static Core& getInstance();
  
  // Gets the REFERENCE to the pid list.
  //
  // Use refresh_pids() to actually update the list.
  std::vector<int>& get_pid_list();

  // Return info related to the passed pid.
  ProcessInfo get_pid_info(int pid);

  // Refreshes the pid list.
  void refresh_pids();

  // Gets the REFERENCE to the SysMemoryUsage class
  // thats containing the basic information about
  // the system memory usage.
  //
  // Use refresh_resources() to update this value.
  SysMemoryInfo& get_memory_usage();

  // Gets the REFERENCE to the list containing
  // information regarding disk usage in bytes.
  //
  // Use refresh_resources() to update this value.
  std::map<std::string, DiskInfo>& get_disk_usage_info();

  // Refreshes the global resource information.
  void refresh_resources();

private:
  Core();

  // Process related stuff
  DIR* proc_dir_;
  std::vector<int> pids_;

  // Resource related stuff
  SysMemoryInfo mem_info_;
  std::ifstream steam_diskstats_;
  struct sysinfo sinfo_;
  std::map<std::string, DiskInfo> disk_info_;
};

}

#endif