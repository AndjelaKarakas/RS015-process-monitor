#include <gtkmm.h>
#include <iostream>
#include <unistd.h>

#include "common/core.h"
#include "windows/mainwindow.h"

int main(int argc, char *argv[]) {
  // TODO: Make separate debug and release configurations
  putenv((char*)"GTK_THEME=Adwaita-dark");

  auto app = Gtk::Application::create(argc, argv, "org.gnome.process-monitor");
  auto window = ProcessMonitor::MainWindow::Create();

  return app->run(*window);



  /* ProcessView interaction example
  
  auto& core = ProcessMonitor::Core::getInstance();
  auto& pids = core.get_pid_list();
  auto& pidinfo = core.get_pid_info();

  while (true) {
    core.refresh_pids();

    for (auto &pid : pids) {
      core.load_pid_info(pid);
      std::cout << pid << " " << pidinfo.name << " " << pidinfo.memory << std::endl;
    }

    // "lazy" pause
    char c;
    std::cin >> c;
  }*/



  /* ResourceView interaction example

  auto& core = ProcessMonitor::Core::getInstance();
  auto& meminfo = core.get_memory_usage();
  auto& diskinfo = core.get_disk_usage_info();

  while (true) {
    core.refresh_resources();

    std::cout << "[Resource Usage]" << std::endl;
    std::cout << "Memory: " << meminfo.memory_usage / 1073741824.0 << " GB / " << meminfo.total_memory / 1073741824.0 << " GB" << std::endl;
    std::cout << "Swap: " << meminfo.swap_usage / 1073741824.0 << " GB / " << meminfo.total_swap / 1073741824.0 << " GB" << std::endl;
    for (auto& info : diskinfo)
      std::cout << "[" << info.first << "] Reads: " << info.second.get_bytes_read() << " Writes: " << info.second.get_bytes_written() << std::endl;
    std::cout << std::endl;

    sleep(1);
  }*/
}
