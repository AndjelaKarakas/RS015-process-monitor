#ifndef PROCESSMONITOR_WIDGETS_RESOURCEVIEW_H_
#define PROCESSMONITOR_WIDGETS_RESOURCEVIEW_H_

#include <gtkmm.h>

#include "graphview.h"

namespace ProcessMonitor {

class ResourceView : public Gtk::Box {
 public:
  static ResourceView* Create();
  ResourceView(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade);

 private:
  Glib::RefPtr<Gtk::Builder> builder_;
  GraphView* graph_view_;
};

}

#endif