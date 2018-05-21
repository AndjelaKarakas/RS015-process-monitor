#include "processview.h"

#include <gtkmm.h>

namespace ProcessMonitor {

ProcessView* ProcessView::Create()
{
  auto builder = Gtk::Builder::create_from_resource("/widgets/processview.glade");
  ProcessMonitor::ProcessView *widget;
  builder->get_widget_derived("processview", widget);

  return widget;
}

ProcessView::ProcessView(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
  Gtk::Widget(cobject),
  builder_(refGlade)
{
  builder_->get_widget("treeprocess", treeprocess_);

  treeprocess_->set_headers_visible(true);

  record_.add(column_pid_);
  record_.add(column_name_);
  record_.add(column_uid_);
  record_.add(column_priority_);
  record_.add(column_trackmemory_);
  record_.add(column_trackcpu_);
  record_.add(column_trackdisk_);

  model_= Gtk::TreeStore::create(record_);
  treeprocess_->set_model(model_);

  treeprocess_->append_column("Pid", column_pid_);
  treeprocess_->append_column("Name", column_name_);
  treeprocess_->append_column("Uid", column_uid_);
  treeprocess_->append_column("Priority", column_priority_);
  treeprocess_->append_column("Track memory", column_trackmemory_);
  treeprocess_->append_column("Track CPU", column_trackcpu_);
  treeprocess_->append_column("Track dick", column_trackdisk_);

  treeprocess_->get_column(0)->set_resizable(true);
  treeprocess_->get_column(0)->set_min_width(100);
  treeprocess_->get_column(1)->set_resizable(true);
  treeprocess_->get_column(1)->set_min_width(100);
  treeprocess_->get_column(2)->set_resizable(true);
  treeprocess_->get_column(2)->set_min_width(100);
  treeprocess_->get_column(3)->set_resizable(true);
  treeprocess_->get_column(3)->set_min_width(100);
  treeprocess_->get_column(4)->set_resizable(true);
  treeprocess_->get_column(4)->set_min_width(100);
  treeprocess_->get_column(5)->set_resizable(true);
  treeprocess_->get_column(5)->set_min_width(100);
  treeprocess_->get_column(6)->set_resizable(true);
  treeprocess_->get_column(6)->set_min_width(100);

  Gtk::TreeModel::Row row = *(model_->append());
  row[column_name_] = "1";
  row[column_pid_] = 2;
  row[column_uid_] = 3;
  row[column_priority_] = 4;
  row[column_trackmemory_] = 5;
  row[column_trackcpu_] = 6;
  row[column_trackdisk_] = 7;
  
  
  
}

}
