#ifndef PROCESSMONITOR_COMMON_NETINFO_H_
#define PROCESSMONITOR_COMMON_NETINFO_H_

#include <fstream>

namespace ProcessMonitor {

// Contains basic info about network usage.
class NetInfo {
  public:
    NetInfo();

    void update();

    // Total bytes downloaded since the last update.
    long get_download_usage();

    // Total bytes uploaded since the last update.
    long get_upload_usage();

  private:
    std::ifstream nstream_;
    long total_in_octets_;
    long total_out_octets_;
    long in_octets_;
    long out_octets_;
    int in_octet_pos_;
    int out_octet_pos_;
};

}

#endif