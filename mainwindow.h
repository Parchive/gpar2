#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/entry.h>
#include <gtkmm.h>
#include "gpar2.h"
#include <libpar2/libpar2.h>
#include <pthread.h>
#include <string.h>
#include<iostream>
#include<sstream>


class MainWindow : public Gtk::Window
{

public:
  MainWindow(char* text);
  virtual ~MainWindow();
  Gtk::ProgressBar progressBar;

protected:
  // Current operation
  typedef enum {
    none,
    repairing,
    verifying,
    scanning
  } Operation;
  Operation operation;

  typedef enum {
    notloaded_repair,
    notloaded_verify,
    notnecessary_repair,
    notpossible_repair,
    already_verified
  } Error;

  typedef enum {
    complete,
    repairable,
    unrepairable,
    undef
  } Status;
  Status status;

  //Signal handlers:
  virtual void verify();
  virtual void repair();
  virtual void open();
  virtual void quit();
  virtual void about();

  void preprocess();
  void update_status(Result result);
  void signal_filename(std::string filename);
  void signal_progress(double progress);
  void signal_headers(ParHeaders* headers);
  void signal_done(std::string filename, int blocks_available, 
		   int blocks_total);
  void errors(Error error);

  LibPar2* repairer;

  // menus
  Glib::RefPtr<Gtk::UIManager> m_refUIManager;
  Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;

  //Member widgets:
  Gtk::VBox main_VBox;
  Gtk::HBox main_HBox;
  Gtk::Button repair_button;
  Gtk::Button verify_button;
  Gtk::Button open_button;
  //Gtk::Entry filename_entry;
  Gtk::Frame status_frame;
  Gtk::Frame done_frame;
  Gtk::Frame headers_frame;
  Gtk::Label status_label;

  // Headers label
  Gtk::HBox headers_hbox;
  Gtk::VBox headers_col1;
  Gtk::VBox headers_col2;
  Gtk::Label header_setid;
  Gtk::Label header_blocksize;
  Gtk::Label header_chunksize;
  Gtk::Label header_sourceblockcount;
  Gtk::Label header_totalsize;
  Gtk::Label header_recoverablefiles;
  Gtk::Label header_otherfiles;
  Gtk::Label header_blocks;

  // Filename
  Gtk::HBox filename_hbox;
  Gtk::Label filename_title;
  Gtk::Label filename_entry;

  Gtk::ProgressBar globalProgress;

  // Files done
  Gtk::ScrolledWindow done_window;
  Gtk::TextView done_files;

  double nbdone;
  double nbfiles;
  bool file_loaded;
  int avail_blocks;
};

// Tools
string itos(int i);	// convert int to string
string dtos(double d);	// convert double to string

#endif // MAINWINDOW_H
