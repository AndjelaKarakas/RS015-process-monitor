#ifndef PROCESSMONITOR_WIDGETS_GRAPHVIEW_H_
#define PROCESSMONITOR_WIDGETS_GRAPHVIEW_H_

#include <gtkmm.h>

namespace ProcessMonitor {

class ResourceView : public Gtk::Box {
 public:
  static ResourceView* Create();
  ResourceView(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade);

 private:
  Glib::RefPtr<Gtk::Builder> builder_;
};

}

#endif