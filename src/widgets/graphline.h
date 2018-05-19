#ifndef PROCESSMONITOR_WIDGETS_GRAPHLINE_H_
#define PROCESSMONITOR_WIDGETS_GRAPHLINE_H_

#include <gtkmm.h>
#include <vector>

namespace ProcessMonitor {

class GraphLine {
 public:
  GraphLine();

  void add_point(double value);
  void draw(const Cairo::RefPtr<Cairo::Context>& context, int& width, int& height);
  void set_color(double red, double green, double blue);
  void set_title(std::string title);
  void resize(double ratio);

  Gtk::Box widget_color_selector_;

 private:
  void color_changed();

  std::vector<double> points_;
  double posx_;
  double red_, green_, blue_;
  Gtk::ColorButton button_color_;
  Gtk::Label label_title_;
  Gtk::Label spacing_right_;
};

}

#endif