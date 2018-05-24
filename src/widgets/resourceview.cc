#include "resourceview.h"

#include <gtkmm.h>

#include "common/core.h"
#include "common/util.h"

namespace ProcessMonitor {

ResourceView* ResourceView::Create() {
  auto builder = Gtk::Builder::create_from_resource("/widgets/resourceview.glade");
  ProcessMonitor::ResourceView *widget;
  builder->get_widget_derived("resourceview", widget);

  return widget;
}

ResourceView::ResourceView(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
  Gtk::ScrolledWindow(cobject),
  builder_(refGlade),
  init_(false),
  network_limit_(1024),
  drive_limit_(1024)
{
  builder_->get_widget("resourceviewbox", resourceviewbox_);

  cpu_view_ = GraphView::Create();
  cpu_view_->set_sizes("0%", "50%", "100%");

  memory_view_ = GraphView::Create();
  memory_view_->add_line("RAM", Util::get_random_color());
  memory_view_->add_line("Swap", Util::get_random_color());

  network_view_ = GraphView::Create();
  network_view_->add_line("Download", Util::get_random_color());
  network_view_->add_line("Upload", Util::get_random_color());

  drive_view_ = GraphView::Create();

  set_props(label_cpu_, "CPU:");
  resourceviewbox_->pack_start(label_cpu_, false, true);
  resourceviewbox_->pack_start(*cpu_view_);

  set_props(label_memory_, "Memory:");
  resourceviewbox_->pack_start(label_memory_, false, true);
  resourceviewbox_->pack_start(*memory_view_);

  set_props(label_network_, "Network:");
  resourceviewbox_->pack_start(label_network_, false, true);
  resourceviewbox_->pack_start(*network_view_);

  set_props(label_drive_, "Drives:");
  resourceviewbox_->pack_start(label_drive_, false, true);
  resourceviewbox_->pack_start(*drive_view_);
}

void ResourceView::set_props(Gtk::Label& label, std::string text) {
  label.set_xalign(0.0);
  label.set_use_markup(true);
  label.set_markup("<b>" + text + "</b>");
}

void ResourceView::fetch() {
  auto& core = Core::getInstance();

  // Get CPU
  auto& cpuinfo = core.get_cpu_usage_info();

  for (auto& pair : cpuinfo)
      if (pair.first != "cpu")
        cpu_view_->add_point(pair.first, pair.second.get_percentage_usage(), Util::percentage_to_string(pair.second.get_percentage_usage()));

  // Get Memory
  auto& memory = core.get_memory_usage();
  auto memory_usage = memory.memory_usage / 1073741824.0;
  auto total_memory = memory.total_memory / 1073741824.0;
  auto swap_usage = memory.swap_usage / 1073741824.0;
  auto total_swap = memory.total_swap / 1073741824.0;

  memory_view_->set_sizes(
    "0 GB / 0 GB",
    Util::bytes_to_string(memory.total_memory / 2) + " / " + Util::bytes_to_string(memory.total_swap / 2),
    Util::bytes_to_string(memory.total_memory) + " / " + Util::bytes_to_string(memory.total_swap)
  );

  memory_view_->add_point("RAM", memory_usage / total_memory, Util::bytes_to_string(memory.memory_usage));
  memory_view_->add_point("Swap", swap_usage / total_swap, Util::bytes_to_string(memory.swap_usage));

  // Get Network info
  auto& network = core.get_network_info();

  if (network.get_download_usage() > network_limit_) {
    network_view_->resize(network.get_download_usage() * 1.0 / network_limit_);
    network_limit_ = network.get_download_usage();
  }

  if (network.get_upload_usage() > network_limit_) {
    network_view_->resize(network.get_upload_usage() * 1.0 / network_limit_);
    network_limit_ = network.get_upload_usage();
  }

  network_view_->add_point("Download", network.get_download_usage() * 1.0 / network_limit_, Util::bytes_to_string(network.get_download_usage()) + "/s");
  network_view_->add_point("Upload", network.get_upload_usage() * 1.0 / network_limit_, Util::bytes_to_string(network.get_upload_usage()) + "/s");

  network_view_->set_sizes(
    "0 KB/s",
    Util::bytes_to_string(network_limit_ / 2) + "/s",
    Util::bytes_to_string(network_limit_) + "/s"
  );

  // Get Drive info
  auto& drive = core.get_disk_usage_info();

  for (auto& drivepair : drive) {
    if (drivepair.second.get_bytes_read() > drive_limit_) {
      drive_view_->resize(drivepair.second.get_bytes_read() * 1.0 / drive_limit_);
      drive_limit_ = drivepair.second.get_bytes_read();
    }

    if (drivepair.second.get_bytes_written() > drive_limit_) {
      drive_view_->resize(drivepair.second.get_bytes_written() * 1.0 / drive_limit_);
      drive_limit_ = drivepair.second.get_bytes_written();
    }
  }

  for (auto& drivepair : drive) {
    drive_view_->add_point(drivepair.first + " read", drivepair.second.get_bytes_read() * 1.0 / drive_limit_, Util::bytes_to_string(drivepair.second.get_bytes_read()) + "/s");
    drive_view_->add_point(drivepair.first + " write", drivepair.second.get_bytes_written() * 1.0 / drive_limit_, Util::bytes_to_string(drivepair.second.get_bytes_written()) + "/s");
  }

  drive_view_->set_sizes(
    "0 KB/s",
    Util::bytes_to_string(drive_limit_ / 2) + "/s",
    Util::bytes_to_string(drive_limit_) + "/s"
  );
}

void ResourceView::update(double timer) {
  memory_view_->update(timer);
  cpu_view_->update(timer);
  network_view_->update(timer);
  drive_view_->update(timer);
}

}