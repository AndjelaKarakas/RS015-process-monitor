#ifndef PROCESSMONITOR_WINDOWS_MAINWINDOW_H_
#define PROCESSMONITOR_WINDOWS_MAINWINDOW_H_

#include <gtkmm.h>

#include "widgets/resourceview.h"
#include "widgets/processview.h"

namespace ProcessMonitor {

class MainWindow : public Gtk::Window {
 public:
  static MainWindow* Create();
  MainWindow(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade);

 private:
  void on_tabbutton1_activate();
  void on_tabbutton2_activate();

  Glib::RefPtr<Gtk::Builder> builder_;
  Gtk::RadioButton* radioprocess_;
  Gtk::RadioButton* radioresources_;
  Gtk::Notebook* notebookmain_;
  ProcessView* processview_;
  ResourceView* resourceview_;
};

}

#endif