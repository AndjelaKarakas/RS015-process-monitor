#ifndef PROCESSMONITOR_COMMON_UTIL_H_
#define PROCESSMONITOR_COMMON_UTIL_H_

#include <gtkmm.h>
#include <iostream>
#include <vector>
#include <map>

namespace ProcessMonitor {

class Util {
 public:
  static Gdk::RGBA get_random_color();
  static std::string bytes_to_string(unsigned long bytes);
  static std::string percentage_to_string(double percentage);
  static std::string& uid_to_string(int uid);
  static std::string pid_to_string(int pid);

 private:
  static double colors_[];
  static int colorpos_;
  static std::map<int, std::string> pidmap_;
  static char array_[];
};

}

#endif