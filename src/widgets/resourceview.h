#ifndef PROCESSMONITOR_WIDGETS_RESOURCEVIEW_H_
#define PROCESSMONITOR_WIDGETS_RESOURCEVIEW_H_

#include <gtkmm.h>

#include "graphview.h"

namespace ProcessMonitor {

class ResourceView : public Gtk::ScrolledWindow {
 public:
  static ResourceView* Create();
  ResourceView(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade);

  void fetch();
  void update(double timer);

 private:
  void set_props(Gtk::Label& label, std::string text);

  Glib::RefPtr<Gtk::Builder> builder_;
  GraphView* cpu_view_;
  GraphView* memory_view_;
  GraphView* network_view_;
  GraphView* drive_view_;
  Gtk::Label label_cpu_, label_memory_, label_network_, label_drive_;
  Gtk::Box* resourceviewbox_;
  bool init_;
  long network_limit_, drive_limit_;
};

}

#endif