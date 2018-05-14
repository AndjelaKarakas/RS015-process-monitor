#include <string>

namespace ProcessMonitor {

// Contains the basic information about a process.
class ProcessInfo {
 public:
  // Name of the process.
  std::string name;

  // Process id number.
  int pid;

  // Memory used (in bytes).
  int memory;

  // Process priority, goes from 0 to 139.
  int priority;

  // Nice level, goes from -20 to 19.
  int nice_level;
};

}