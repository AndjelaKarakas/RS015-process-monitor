#ifndef PROCESSMONITOR_WIDGETS_GRAPHLINE_H_
#define PROCESSMONITOR_WIDGETS_GRAPHLINE_H_

#include <sys/time.h>
#include <gtkmm.h>
#include <vector>

namespace ProcessMonitor {

class GraphLine {
 public:
  GraphLine();

  void add_point(double value, std::string display_value);
  void draw(const Cairo::RefPtr<Cairo::Context>& context, int& width, int& height);
  void set_color(double red, double green, double blue);
  void set_color(Gdk::RGBA rgba);
  void set_title(std::string title);
  void resize(double ratio);
  void update(int& timer);

  Gtk::Box widget_color_selector_;

 private:
  void color_changed();

  std::vector<double> points_;
  int posx_;
  Gdk::RGBA rgba_;
  Gtk::ColorButton button_color_;
  Gtk::Label label_title_;
  Gtk::Label spacing_right_;
  std::string title_;
};

}

#endif