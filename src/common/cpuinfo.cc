#include "cpuinfo.h"

namespace ProcessMonitor {

CpuInfo::CpuInfo() :
  total_used_jifs_(-1),
  total_free_jifs_(-1),
  used_jifs_(0),
  free_jifs_(1) {

}

CpuInfo::CpuInfo(long total_used_jifs, long total_free_jifs) : CpuInfo() {
  update(total_used_jifs, total_free_jifs);
}

double CpuInfo::get_total_jifs() {
  return used_jifs_ + free_jifs_;
}

double CpuInfo::get_percentage_usage() {
  return (100.0 * used_jifs_) / (used_jifs_ + free_jifs_);
}

void CpuInfo::update(long total_used_jifs, long total_free_jifs) {
  if (total_used_jifs_ != -1) {
    used_jifs_ = total_used_jifs - total_used_jifs_;
    free_jifs_ = total_free_jifs - total_free_jifs_;
  }

  total_used_jifs_ = total_used_jifs;
  total_free_jifs_ = total_free_jifs;
}

}