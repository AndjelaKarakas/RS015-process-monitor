#include "netinfo.h"


namespace ProcessMonitor {

NetInfo::NetInfo() :
  nstream_("/proc/net/netstat"),
  total_in_octets_(-1),
  total_out_octets_(-1),
  in_octets_(0),
  out_octets_(0),
  in_octet_pos_(-1),
  out_octet_pos_(-1) {
  
}

void NetInfo::update() {
  if (in_octet_pos_ == -1) { // First time, lets prep the info
    std::string lastread;
    int counter = 0;

    nstream_.ignore(9999, '\n');
    nstream_.ignore(9999, '\n');

    while (true)
    {
      nstream_ >> lastread;
      if (lastread == "InOctets")
        in_octet_pos_ = counter;
      else if (lastread == "OutOctets")
        out_octet_pos_ = counter;

      if (in_octet_pos_ != -1 && out_octet_pos_ != -1)
        break;

      counter++;
    }
  }

  nstream_.clear();
  nstream_.seekg(0, std::ios::beg);
  nstream_.ignore(9999, '\n');
  nstream_.ignore(9999, '\n');
  nstream_.ignore(9999, '\n');

  int i = 0;
  long total_in_octets, total_out_octets;

  while (i <= in_octet_pos_ || i <= out_octet_pos_) {
    if (i == in_octet_pos_) {
      nstream_ >> total_in_octets;
      nstream_.ignore(1);
    } else if (i == out_octet_pos_) {
      nstream_ >> total_out_octets;
      nstream_.ignore(1);
    } else {
      nstream_.ignore(255, ' ');
    }

    i++;
  }

  if (total_in_octets_ != -1) {
    in_octets_ = total_in_octets - total_in_octets_;
    out_octets_ = total_out_octets - total_out_octets_;
  }

  total_in_octets_ = total_in_octets;
  total_out_octets_ = total_out_octets;
}

long NetInfo::get_download_usage() {
  return in_octets_;
}

long NetInfo::get_upload_usage() {
  return out_octets_;
}

}