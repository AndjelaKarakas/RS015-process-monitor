#include "diskinfo.h"

namespace ProcessMonitor {

DiskInfo::DiskInfo() :
  reads_(0),
  writes_(0),
  total_reads_(-1),
  total_writes_(-1) {

}

DiskInfo::DiskInfo(long total_reads, long total_writes) : DiskInfo() {
  update(total_reads, total_writes);
}

long DiskInfo::get_bytes_read() {
  return reads_ << 9;
}

long DiskInfo::get_bytes_written() {
  return writes_ << 9;
}

void DiskInfo::update(long total_reads, long total_writes) {
  if (total_reads_ != -1 && total_writes_ != -1) {
    reads_ = total_reads - total_reads_;
    writes_ = total_writes - total_writes_;
  }

  total_reads_ = total_reads;
  total_writes_ = total_writes;
}

}