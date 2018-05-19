#include "resourceview.h"

#include <gtkmm.h>

namespace ProcessMonitor {

ResourceView* ResourceView::Create()
{
  auto builder = Gtk::Builder::create_from_resource("/widgets/resourceview.glade");
  ProcessMonitor::ResourceView *widget;
  builder->get_widget_derived("resourceview", widget);

  return widget;
}

ResourceView::ResourceView(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
  Gtk::Box(cobject),
  builder_(refGlade)
{
  graph_view_ = GraphView::Create();
  graph_view_->set_sizes("0", "9000", "over 9000");

  graph_view_->add_line("RAM", 1.0, 0.0, 0.0);
  graph_view_->add_line("Swap", 0.0, 1.0, 0.0);

  for(int i = 0; i < 10; i++) {
    graph_view_->add_point("RAM", 0.5);
    graph_view_->add_point("RAM", 0.25);
    graph_view_->add_point("RAM", 0.3);
    graph_view_->add_point("RAM", 0.75);
    graph_view_->add_point("RAM", 0.75);
    graph_view_->add_point("RAM", 0.75);

    graph_view_->add_point("Swap", 0.0);
    graph_view_->add_point("Swap", 0.3);
    graph_view_->add_point("Swap", 0.75);
    graph_view_->add_point("Swap", 0.25);
    graph_view_->add_point("Swap", 0.25);
    graph_view_->add_point("Swap", 0.25);
  }
  graph_view_->resize(2.0);

  pack_start(*graph_view_);
}

}