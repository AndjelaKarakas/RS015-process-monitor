#include "processinfo.h"

#include "core.h"
#include <iostream>

namespace ProcessMonitor {
  
ProcessInfo::ProcessInfo() :
  total_cpu_jits_(-1),
  cpu_jits_(0),
  total_bytes_read_(0),
  bytes_read_(0),
  total_bytes_written_(0),
  bytes_written_(0) {
  
}

long ProcessInfo::get_bytes_read() {
  return bytes_read_;
}

long ProcessInfo::get_bytes_written() {
  return bytes_written_;
}

double ProcessInfo::get_cpu_percentage_usage() {
  return (cpu_jits_ * 100.0) / Core::getInstance().get_cpu_usage_info()["cpu"].get_total_jifs();
}
  
void ProcessInfo::update(long cpu_jits, long bytes_read, long bytes_written) {
  if (total_cpu_jits_ != -1)
  {
    cpu_jits_ = cpu_jits - total_cpu_jits_;
    bytes_read_ = bytes_read - total_bytes_read_;
    bytes_written_ = bytes_written - total_bytes_written_;
  }

  total_cpu_jits_ = cpu_jits;
  total_bytes_read_ = bytes_read;
  total_bytes_written_ = bytes_written;
}

}
