#include "util.h"

#include <sys/types.h>
#include <libgen.h>
#include <iomanip>
#include <pwd.h>
#include <unistd.h>

namespace ProcessMonitor {

int Util::colorpos_ = 0;

double Util::colors_[] = { 
  0.6, 0.05, 0.07,
  0.76, 0.37, 0.09,
  0.73, 0.63, 0.08,
  0.32, 0.61, 0.06,
  0.18, 0.27, 0.52,
  0.45, 0.30, 0.47,
  0.53, 0.36, 0.07
};

std::map<int, std::string> Util::pidmap_;

char Util::array_[1024];

Gdk::RGBA Util::get_random_color() {
  colorpos_ += 3;
  if (colorpos_ >= (int)(sizeof(colors_) / sizeof(colors_[0])))
    colorpos_ = 0;

  Gdk::RGBA ret;
  ret.set_rgba(colors_[colorpos_], colors_[colorpos_ + 1], colors_[colorpos_ + 2]);
  return ret;
}

std::string Util::bytes_to_string(unsigned long bytes) {
  std::stringstream ret;

  if (bytes < 1024)
    ret << bytes << " B";
  else if (bytes < 1024 * 1024)
    ret << std::fixed << std::setprecision(0) << bytes / 1024.0 << " KB";
  else if (bytes < 1024 * 1024 * 512)
    ret << std::fixed << std::setprecision(2) << bytes / (1024.0 * 1024.0) << " MB";
  else
    ret << std::fixed << std::setprecision(2) << bytes / (1024.0 * 1024.0 * 1024.0) << " GB";

  return ret.str();
}

std::string Util::percentage_to_string(double percentage) {
  std::stringstream ret;
  ret << std::fixed << std::setprecision(1) << percentage * 100.0 << "%";

  return ret.str();
}

std::string& Util::uid_to_string(int uid) {
  auto item = pidmap_.find(uid);

  if (item == pidmap_.end()) {
    struct passwd *pwd = getpwuid(uid);
    pidmap_[uid] = std::string(pwd->pw_name);

    item = pidmap_.find(uid);
  }

  return item->second;
}

std::string Util::pid_to_string(int pid) {
  auto pathname = "/proc/" + std::to_string(pid) + "/exe";
  auto link = readlink(pathname.c_str(), array_, 1024);
  array_[link] = '\0';

  return std::string(basename(array_));
}

}