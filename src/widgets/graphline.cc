#include "graphline.h"

namespace ProcessMonitor {

GraphLine::GraphLine() :
  red_(0.0),
  green_(0.0),
  blue_(0.0) {
  button_color_.signal_color_set().connect(
    sigc::mem_fun(*this, &GraphLine::color_changed)
  );

  widget_color_selector_.pack_start(label_title_, false, true);
  widget_color_selector_.pack_start(button_color_);
  widget_color_selector_.pack_start(spacing_right_);
}

void GraphLine::color_changed() {
  auto color = button_color_.get_rgba();
  
  red_ = color.get_red();
  green_ = color.get_green();
  blue_ = color.get_blue();
}

void GraphLine::add_point(double value) {
  if (points_.size() == 0)
    posx_ = 1.0;

  points_.push_back(value);
}

void GraphLine::draw(const Cairo::RefPtr<Cairo::Context>& context, int& width, int& height) {
  if (points_.size() == 0)
    return;

  context->set_source_rgb(red_, green_, blue_);
  context->set_line_width(2);

  double pox = posx_;
  context->move_to(pox * width, points_[0] * height);

  for (auto& point : points_) {
    context->line_to(pox * width, point * height);

    pox += 0.1;
  }

  if (posx_ < -0.1)
  {
    points_.erase(points_.begin());
    posx_ += 0.1;
  }
  
  posx_ -= 0.005;
  context->stroke();
}

void GraphLine::set_color(double red, double green, double blue) {
  red_ = red;
  green_ = green;
  blue_ = blue;
  
  Gdk::RGBA rgba;
  rgba.set_rgba(red, green, blue);
  button_color_.set_rgba(rgba);
}

void GraphLine::set_title(std::string title) {
  label_title_.set_text(title + ": ");
}

void GraphLine::resize(double ratio) {
  for (uint i = 0; i < points_.size(); i++)
    points_[i] = 1.0 - ((1.0 - points_[i]) / ratio);
}

}