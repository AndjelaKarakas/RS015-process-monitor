#ifndef PROCESSMONITOR_COMMON_UTIL_H_
#define PROCESSMONITOR_COMMON_UTIL_H_

#include <gtkmm.h>
#include <iostream>
#include <vector>

namespace ProcessMonitor {

class Util {
 public:
  static Gdk::RGBA get_random_color();
  static std::string bytes_to_string(unsigned long bytes);
  static std::string percentage_to_string(double percentage);

 private:
  static double colors_[];
  static int colorpos_;
};

}

#endif