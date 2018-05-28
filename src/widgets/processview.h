#ifndef PROCESSMONITOR_WIDGETS_PROCESSVIEW_H_
#define PROCESSMONITOR_WIDGETS_PROCESSVIEW_H_

#include <gtkmm.h>
#include <map>

namespace ProcessMonitor {

class ProcessView : public Gtk::Widget {
 public:
  static ProcessView* Create();
  ProcessView(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade);

  void update();

 private:
  Glib::RefPtr<Gtk::Builder> builder_;
  Gtk::TreeView* treeprocess_;
  std::map<int, Gtk::TreeModel::Row> pidmap_;
  Glib::RefPtr<Gtk::TreeStore> model_;
  Gtk::TreeModel::ColumnRecord record_;
  Gtk::TreeModelColumn<Glib::ustring> column_name_;
  Gtk::TreeModelColumn<uint> column_pid_;
  Gtk::TreeModelColumn<Glib::ustring> column_uid_;
  Gtk::TreeModelColumn<int> column_priority_;
  Gtk::TreeModelColumn<Glib::ustring> column_trackmemory_;
  Gtk::TreeModelColumn<int> column_trackcpu_;
  Gtk::TreeModelColumn<Glib::ustring> column_trackdisk_;
  Gtk::TreeModelColumn<long> column_modelmemory_;
  Gtk::TreeModelColumn<int> column_modelcpu_;
};

}

#endif