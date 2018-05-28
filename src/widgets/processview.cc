#include "processview.h"

#include <gtkmm.h>

#include "common/core.h"
#include "common/util.h"

namespace ProcessMonitor {

ProcessView* ProcessView::Create() {
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
  record_.add(column_modelmemory_);
  record_.add(column_modelcpu_);

  model_= Gtk::TreeStore::create(record_);
  treeprocess_->set_model(model_);

  treeprocess_->append_column("Pid", column_pid_);
  treeprocess_->append_column("Name", column_name_);
  treeprocess_->append_column("Uid", column_uid_);
  treeprocess_->append_column("Priority", column_priority_);
  treeprocess_->append_column("Track memory", column_trackmemory_);
  treeprocess_->append_column("Track CPU", column_trackcpu_);
  treeprocess_->append_column("Track dick", column_trackdisk_);
  
  treeprocess_->get_column(0)->set_sort_column(column_pid_);
  treeprocess_->get_column(1)->set_sort_column(column_name_);
  treeprocess_->get_column(2)->set_sort_column(column_uid_);
  treeprocess_->get_column(3)->set_sort_column(column_priority_);
  treeprocess_->get_column(4)->set_sort_column(column_modelmemory_);
  treeprocess_->get_column(5)->set_sort_column(column_modelcpu_);
  treeprocess_->get_column(6)->set_sort_column(column_trackdisk_);

  for (int i = 0; i < 7; i++) {
    treeprocess_->get_column(i)->set_resizable(true);
    treeprocess_->get_column(i)->set_min_width(100);
  }
}

void ProcessView::update() {
  auto& core = Core::getInstance();
  auto& list = core.get_pid_list();

  // add missing pids
  for (auto& item : list)
    if (pidmap_.find(item) == pidmap_.end())
      pidmap_[item] = *(model_->append());

  // update all pids and remove
  for (auto& item : pidmap_) {
    if (core.valid_pid(item.first)) {
      auto& info = core.get_pid_info(item.first);
      auto& row = item.second;

      row[column_name_] = info.name;
      row[column_pid_] = info.pid;
      row[column_uid_] = Util::uid_to_string(info.uid);
      row[column_priority_] = info.priority;
      row[column_trackmemory_] = Util::bytes_to_string(info.memory);
      row[column_modelmemory_] = -info.memory;
      row[column_trackcpu_] = info.get_cpu_percentage_usage();
      row[column_modelcpu_] = -info.get_cpu_percentage_usage();
      row[column_trackdisk_] = Util::bytes_to_string(info.get_bytes_read() + info.get_bytes_written());
    } else {
      model_->erase(item.second);
      pidmap_.erase(item.first);
    }
  }
}

}
