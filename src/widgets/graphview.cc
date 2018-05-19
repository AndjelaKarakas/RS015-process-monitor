#include "graphview.h"

#include <iostream>
#include <gtkmm.h>

namespace ProcessMonitor {

GraphView* GraphView::Create() {
  auto builder = Gtk::Builder::create_from_resource("/widgets/graphview.glade");
  ProcessMonitor::GraphView *widget;
  builder->get_widget_derived("graphview", widget);

  return widget;
}

GraphView::GraphView(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
  Gtk::Box(cobject),
  builder_(refGlade)
{
  builder_->get_widget("graphdrawarea", drawing_area_);
  builder_->get_widget("colorbox", colorbox_);
  builder_->get_widget("labelmin", label_min_);
  builder_->get_widget("labelmiddle", label_middle_);
  builder_->get_widget("labelmax", label_max_);

  drawing_area_->signal_draw().connect(
    sigc::mem_fun(*this, &GraphView::draw_graph)
  );
}

bool GraphView::draw_graph(const Cairo::RefPtr<Cairo::Context>& context) {
  drawing_area_->queue_draw();

  auto draw_width = drawing_area_->get_allocated_width();
  auto draw_height = drawing_area_->get_allocated_height();

  context->set_source_rgba(1.0, 1.0, 1.0, 1.0);
  context->rectangle(0, 0, draw_width, draw_height);
  context->fill();

  // Draw lines
  context->set_source_rgba(0.5, 0.5, 0.5, 1.0);
  context->set_line_width(1);

  context->move_to(0.0, draw_height / 4);
  context->line_to(draw_width, draw_height / 4);

  context->move_to(0.0, draw_height / 2);
  context->line_to(draw_width, draw_height / 2);

  context->move_to(0.0, draw_height * 3 / 4);
  context->line_to(draw_width, draw_height * 3 / 4);

  context->stroke();

  for (auto& pair : lines_)
    pair.second.draw(context, draw_width, draw_height);

  // Draw frame
  context->set_source_rgba(0.0, 0.0, 0.0, 1.0);
  context->set_line_width(2);

  context->move_to(0.0, 1.0);
  context->line_to(draw_width - 1.0, 0.0);
  context->line_to(draw_width - 1.0, draw_height - 1.0);
  context->line_to(1.0, draw_height - 1.0);
  context->line_to(1.0, 0.0);

  context->stroke();

  return true;
}

void GraphView::add_line(std::string line, double red, double green, double blue) {
  GraphLine& gline = lines_[line];
  gline.set_color(red, green, blue);
  gline.set_title(line);

  colorbox_->pack_start(gline.widget_color_selector_);
  colorbox_->show_all();
}

void GraphView::add_point(std::string line, double value) {
  lines_[line].add_point(1.0 - value);
}

void GraphView::set_color(std::string line, double red, double green, double blue) {
  lines_[line].set_color(red, green, blue);
}

void GraphView::resize(double ratio) {
  for (auto& pair : lines_)
    pair.second.resize(ratio);
}

void GraphView::set_sizes(std::string min, std::string middle, std::string max) {
  label_min_->set_label(min);
  label_middle_->set_text(middle);
  label_max_->set_text(max);
}

}