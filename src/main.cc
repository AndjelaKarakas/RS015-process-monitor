#include <gtkmm.h>
#include <iostream>
#include <unistd.h>

#include "common/core.h"
#include "windows/mainwindow.h"

int main(int argc, char *argv[]) {
  // TODO: Make separate debug and release configurations
  putenv((char*)"GTK_THEME=Adwaita-dark");

  auto app = Gtk::Application::create("org.gnome.process-monitor");
  app->register_application();

  auto window = ProcessMonitor::MainWindow::Create();
  app->add_window(*window);

  // Create app menu
  auto menu = Gio::Menu::create();
  menu->append_item(Gio::MenuItem::create("About", "app.about"));
  menu->append_item(Gio::MenuItem::create("Quit", "app.quit"));
  app->set_app_menu(menu);

  // Connect signals to app menu
  
  window->connect_app_menu(app);
  /*auto about_item = Gio::SimpleAction::create("about");
  about_item->signal_activate().connect(
    sigc::mem_fun(nullptr, about_activated)
  );*/

  window->show_all();
  return app->run(*window);//*/



  /* ProcessView interaction example
  auto& core = ProcessMonitor::Core::getInstance();
  auto& pids = core.get_pid_list();

  while (true) {
    core.refresh();

    for (auto &pid : pids) {
      auto& pidinfo = core.get_pid_info(pid);
      std::cout << pid << " " << pidinfo.name << " " << pidinfo.uid << " " << pidinfo.get_cpu_percentage_usage() << " " << pidinfo.memory << " " << pidinfo.get_bytes_read() << " " << pidinfo.get_bytes_written() << std::endl;
    }

    // "lazy" pause
    char c;
    std::cin >> c;
  }//*/



  /* ResourceView interaction example

  auto& core = ProcessMonitor::Core::getInstance();
  auto& meminfo = core.get_memory_usage();
  auto& diskinfo = core.get_disk_usage_info();
  auto& cpuinfo = core.get_cpu_usage_info();
  auto& netinfo = core.get_network_info();

  while (true) {
    core.refresh();

    std::cout << "[Resource Usage]" << std::endl;
    std::cout << "Memory: " << meminfo.memory_usage / 1073741824.0 << " GB / " << meminfo.total_memory / 1073741824.0 << " GB" << std::endl;
    std::cout << "Swap: " << meminfo.swap_usage / 1073741824.0 << " GB / " << meminfo.total_swap / 1073741824.0 << " GB" << std::endl;
    for (auto& info : diskinfo)
      std::cout << "[" << info.first << "] Reads: " << info.second.get_bytes_read() << " Writes: " << info.second.get_bytes_written() << std::endl;
    for (auto& cpu : cpuinfo)
      std::cout << "[" << cpu.first << "]: " << cpu.second.get_percentage_usage() << std::endl;
    std::cout << "Download: " << netinfo.get_download_usage() / 1024.0 << " KB" << std::endl;
    std::cout << "Upload: " << netinfo.get_upload_usage() / 1024.0 << " KB" << std::endl;
    std::cout << std::endl;

    sleep(1);
  }//*/
}
