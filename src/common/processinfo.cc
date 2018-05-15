#include "processinfo.h"

#include "core.h"
#include <iostream>

namespace ProcessMonitor {
  
ProcessInfo::ProcessInfo() :
  total_cpu_jits_(-1),
  cpu_jits_(0) {
  
}

double ProcessInfo::get_cpu_percentage_usage() {
  return (cpu_jits_ * 100.0) / Core::getInstance().get_cpu_usage_info()["cpu"].get_total_jifs();
}
  
void ProcessInfo::update(long cpu_jits) {
  if (total_cpu_jits_ != -1)
    cpu_jits_ = cpu_jits - total_cpu_jits_;

  total_cpu_jits_ = cpu_jits;
}

}
