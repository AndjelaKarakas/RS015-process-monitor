#include "graphline.h"

namespace ProcessMonitor {

GraphLine::GraphLine() {
  button_color_.signal_color_set().connect(
    sigc::mem_fun(*this, &GraphLine::color_changed)
  );

  widget_color_selector_.pack_start(label_title_, false, true);
  widget_color_selector_.pack_start(button_color_);
  widget_color_selector_.pack_start(spacing_right_);
}

void GraphLine::color_changed() {
  rgba_ = button_color_.get_rgba();
}

void GraphLine::add_point(double value, std::string display_value) {
  if (points_.size() == 0)
    posx_ = 61000;

  points_.push_back(value);
  label_title_.set_text(title_ + " (" + display_value + "): ");
}

void GraphLine::update(int& timer) {
  if (posx_ < -1000)
  {
    points_.erase(points_.begin());
    posx_ += 1000;
  }

  posx_ -= timer;
}

void GraphLine::draw(const Cairo::RefPtr<Cairo::Context>& context, int& width, int& height) {
  if (points_.size() == 0)
    return;

  context->set_source_rgb(rgba_.get_red(), rgba_.get_green(), rgba_.get_blue());
  context->set_line_width(2);

  int pox = posx_;
  context->move_to(pox / 60000.0 * width, 2 + points_[0] * (height - 4));

  for (auto& point : points_) {
    context->line_to(pox / 60000.0 * width, 2 + point * (height - 4));

    pox += 1000;
  }

  context->stroke();
}

void GraphLine::set_color(Gdk::RGBA rgba) {
  rgba_ = rgba;
  button_color_.set_rgba(rgba);
}

void GraphLine::set_color(double red, double green, double blue) {
  Gdk::RGBA rgba;
  rgba.set_rgba(red, green, blue);
  set_color(rgba);
}

void GraphLine::set_title(std::string title) {
  title_ = title;
  label_title_.set_text(title + ": ");
}

void GraphLine::resize(double ratio) {
  for (uint i = 0; i < points_.size(); i++)
    points_[i] = 1.0 - ((1.0 - points_[i]) / ratio);
}

}