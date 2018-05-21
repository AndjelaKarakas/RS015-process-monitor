#ifndef PROCESSMONITOR_WIDGETS_PROCESSVIEW_H_
#define PROCESSMONITOR_WIDGETS_PROCESSVIEW_H_

#include <gtkmm.h>

namespace ProcessMonitor {

class ProcessView : public Gtk::Widget {
 public:
  static ProcessView* Create();
  ProcessView(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade);

 private:
  Glib::RefPtr<Gtk::Builder> builder_;
  Gtk::TreeView* treeprocess_;

  Glib::RefPtr<Gtk::TreeStore> model_;
  Gtk::TreeModel::ColumnRecord record_;
  Gtk::TreeModelColumn<Glib::ustring> column_name_;
  Gtk::TreeModelColumn<uint> column_pid_;
  Gtk::TreeModelColumn<uint> column_uid_;
  Gtk::TreeModelColumn<int> column_priority_;
  Gtk::TreeModelColumn<ulong> column_trackmemory_;
  Gtk::TreeModelColumn<int> column_trackcpu_;
  Gtk::TreeModelColumn<ulong> column_trackdisk_;
  
  
};

}

#endif