#include "mainwindow.h"

#include <iostream>
#include <gtkmm.h>

#include "widgets/resourceview.h"
#include "widgets/processview.h"

namespace ProcessMonitor {

MainWindow* MainWindow::Create() {
  auto builder = Gtk::Builder::create_from_resource("/windows/mainwindow.glade");
  ProcessMonitor::MainWindow *window;
  builder->get_widget_derived("mainwindow", window);

  return window;
}

MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
  Gtk::Window(cobject),
  builder_(refGlade)
{
  // init MainWindow data
  builder_->get_widget("radioprocess", radioprocess_);
  builder_->get_widget("radioresources", radioresources_);
  builder_->get_widget("notebookmain", notebookmain_);

  // fill up notebook
  processview_ = ProcessView::Create();
  notebookmain_->append_page(*processview_);

  resourceview_ = ResourceView::Create();
  notebookmain_->append_page(*resourceview_);

  // connect events
  radioprocess_->signal_clicked().connect(
    sigc::mem_fun(*this, &MainWindow::on_tabbutton1_activate)
  );
  radioresources_->signal_clicked().connect(
    sigc::mem_fun(*this, &MainWindow::on_tabbutton2_activate)
  );
}

void MainWindow::connect_app_menu(Glib::RefPtr<Gtk::Application> app) {
  app_ = app;

  auto about_item = Gio::SimpleAction::create("about");
  about_item->signal_activate().connect(
    sigc::mem_fun(*this, &MainWindow::about_activated)
  );
  app->add_action(about_item);

  auto quit_item = Gio::SimpleAction::create("quit");
  quit_item->signal_activate().connect(
    sigc::mem_fun(*this, &MainWindow::quit_activated)
  );
  app->add_action(quit_item);
}

void MainWindow::on_tabbutton1_activate() {
  notebookmain_->set_current_page(0);
}

void MainWindow::on_tabbutton2_activate() {
  notebookmain_->set_current_page(1);
}

void MainWindow::about_activated(const Glib::VariantBase& arg) {
  Gtk::AboutDialog dialog;
  

  dialog.set_transient_for(*this);

  dialog.set_program_name("Process Monitor");
  dialog.set_version("Version: 1.0.0");
  dialog.set_license("Process Monitor is licenced under the MIT license");
  dialog.set_comments("Process Monitor is a simple, Gtk based, process monitor for Linux!");
  dialog.set_website("https://github.com/MATF-RS18/RS015-process-monitor");
  dialog.set_website_label("GitHub Page");

  std::vector<Glib::ustring> list_authors;
  list_authors.push_back("Jeremic Marko");
  list_authors.push_back("Simic Zeljko");
  list_authors.push_back("Karakas Andjela");
  dialog.set_authors(list_authors);

  dialog.run();
}

void MainWindow::quit_activated(const Glib::VariantBase& arg) {
  app_->quit();
}

}
