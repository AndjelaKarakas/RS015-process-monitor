#ifndef PROCESSMONITOR_COMMON_DISKINFO_H_
#define PROCESSMONITOR_COMMON_DISKINFO_H_

#include <string>

namespace ProcessMonitor {

// Contains basic information about disk I/O.
//
// 1 sector is always equal to 512 bytes.
// (its hardcoded in some parts of Linux kernel)
// (also WHYYY?)
class DiskInfo {
  public:
    DiskInfo();
    DiskInfo(long total_reads, long total_writes);

    // Numbers of bytes read since the last call.
    long get_bytes_read();
    // Number of bytes written since the last call.
    long get_bytes_written();

    void update(long total_reads, long total_writes);

  private:
    // Number of sector reads since the last call.
    long reads_;
    // Number of sector writes since the last call.
    long writes_;
    // Total number of sectors read.
    long total_reads_;
    // Total number of sectors written.
    long total_writes_;
};

}

#endif