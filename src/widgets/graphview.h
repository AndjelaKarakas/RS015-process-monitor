#ifndef PROCESSMONITOR_WIDGETS_GRAPHVIEW_H_
#define PROCESSMONITOR_WIDGETS_GRAPHVIEW_H_

#include <sys/time.h>
#include <gtkmm.h>
#include <map>

#include "graphline.h"

namespace ProcessMonitor {

class GraphView : public Gtk::Box {
 public:
  static GraphView* Create();
  GraphView(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade);
  void add_line(std::string line, double red, double green, double blue);
  void add_line(std::string line, Gdk::RGBA rgba);
  // void remove_line(std::string line);
  void add_point(std::string line, double value, std::string display_value);
  void set_color(std::string line, double red, double green, double blue);
  void set_color(std::string line, Gdk::RGBA rgba);
  void resize(double ratio);
  void set_sizes(std::string min, std::string middle, std::string max);
  void update(int timer);

 private:
  bool draw_graph(const Cairo::RefPtr<Cairo::Context>& context);

  Glib::RefPtr<Gtk::Builder> builder_;
  Gtk::DrawingArea* drawing_area_;
  Gtk::Box* colorbox_;
  std::map<std::string, GraphLine> lines_;
  Gtk::Label* label_min_;
  Gtk::Label* label_middle_;
  Gtk::Label* label_max_;
};


}

#endif