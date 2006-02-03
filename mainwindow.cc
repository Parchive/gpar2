#include "mainwindow.h"
#include <libintl.h>

using namespace Glib;

MainWindow::MainWindow(char* text)
  : header_setid(_("Set ID: ")),
    header_blocksize(_("Block size: ")),
    //header_chunksize(_("Chunk size: ")),
    header_sourceblockcount(_("Data Blocks: ")),
    header_totalsize(_("Data size: ")),
    header_recoverablefiles(_("Recoverable files: ")),
    header_otherfiles(_("Other files: ")),
    //header_blocks(_("Blocks: ")),
    filename_title(_("File: "))
{
  operation = none;
  nbdone = 0.0;
  set_title("GPar2");
  set_size_request(600, 350);
  add(main_VBox);

  // Menus
  //Create actions for menus and toolbars:
  m_refActionGroup = Gtk::ActionGroup::create();

  //File menu:
  m_refActionGroup->add( Gtk::Action::create("FileMenu", _("File")) );
  m_refActionGroup->add( Gtk::Action::create("FileOpen", Gtk::Stock::OPEN), 
			 sigc::mem_fun(*this, &MainWindow::open));
  m_refActionGroup->add( Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
			 sigc::mem_fun(*this, &MainWindow::quit) );

  //Action menu:
  m_refActionGroup->add( Gtk::Action::create("ActionMenu", _("Actions")) );
  m_refActionGroup->add( Gtk::Action::create("ActionRepair",Gtk::Stock::EXECUTE, _("Repair")),
			 Gtk::AccelKey("<control>R"),
			 sigc::mem_fun(*this, &MainWindow::repair));
  m_refActionGroup->add( Gtk::Action::create("ActionVerify",Gtk::Stock::APPLY, _("Verify")),
			 Gtk::AccelKey("<control>V"),
			 sigc::mem_fun(*this, &MainWindow::verify));

  //Help menu:
  m_refActionGroup->add( Gtk::Action::create("HelpMenu", _("Help")) );
  m_refActionGroup->add( Gtk::Action::create("HelpAbout", Gtk::Stock::ABOUT),
			 sigc::mem_fun(*this, &MainWindow::about) );

  m_refUIManager = Gtk::UIManager::create();
  m_refUIManager->insert_action_group(m_refActionGroup);
 
  add_accel_group(m_refUIManager->get_accel_group());

  //Layout the actions in a menubar and toolbar:
  try
  {
    Glib::ustring ui_info = 
        "<ui>"
        "  <menubar name='MenuBar'>"
        "    <menu action='FileMenu'>"
        "      <menuitem action='FileOpen'/>"
        "      <separator/>"
        "      <menuitem action='FileQuit'/>"
        "    </menu>"
        "    <menu action='ActionMenu'>"
        "      <menuitem action='ActionVerify'/>"
        "      <menuitem action='ActionRepair'/>"
        "    </menu>"
        "    <menu action='HelpMenu'>"
        "      <menuitem action='HelpAbout'/>"
        "    </menu>"
        "  </menubar>"
        "  <toolbar  name='ToolBar'>"
        "    <toolitem action='FileOpen'/>"
        "    <separator/>"
        "    <toolitem action='ActionVerify'/>"
        "    <toolitem action='ActionRepair'/>"
        "  </toolbar>"
        "</ui>";
        
    m_refUIManager->add_ui_from_string(ui_info);
  }
  catch(const Glib::Error& ex)
  {
    std::cerr << "building menus failed: " <<  ex.what();
  }

 
  //Get the menubar and toolbar widgets, and add them to a container widget:
  Gtk::Widget* pMenubar = m_refUIManager->get_widget("/MenuBar");
  if(pMenubar)
    main_VBox.pack_start(*pMenubar, Gtk::PACK_SHRINK);

  Gtk::Widget* pToolbar = m_refUIManager->get_widget("/ToolBar") ;
  if(pToolbar)
    main_VBox.pack_start(*pToolbar, Gtk::PACK_SHRINK);

  // Filename
  main_VBox.pack_start(filename_hbox);
  filename_hbox.pack_start(filename_title, Gtk::PACK_SHRINK);
  filename_hbox.pack_start(filename_entry);
  filename_entry.set_text(text);
  filename_entry.set_alignment(Gtk::ALIGN_LEFT);
  if (!strcmp(text, ""))
    filename_title.set_sensitive(false);
  
  // Headers frame 
  main_VBox.pack_start(headers_frame);
  headers_frame.set_label(_("Headers"));
  headers_frame.add(headers_hbox);
  headers_hbox.pack_start(headers_col1);
  headers_hbox.pack_start(headers_col2);
  headers_col1.pack_start(header_setid);
  header_setid.set_sensitive(false);
  header_setid.set_alignment(Gtk::ALIGN_LEFT);
  headers_col1.pack_start(header_blocksize);
  header_blocksize.set_sensitive(false);
  header_blocksize.set_alignment(Gtk::ALIGN_LEFT);
  //headers_col1.pack_start(header_chunksize);
  //header_chunksize.set_sensitive(false);
  //header_chunksize.set_alignment(Gtk::ALIGN_LEFT);
  headers_col1.pack_start(header_sourceblockcount);
  header_sourceblockcount.set_sensitive(false);
  header_sourceblockcount.set_alignment(Gtk::ALIGN_LEFT);

  headers_col2.pack_start(header_totalsize);
  header_totalsize.set_sensitive(false);
  header_totalsize.set_alignment(Gtk::ALIGN_LEFT);
  headers_col2.pack_start(header_recoverablefiles);
  header_recoverablefiles.set_sensitive(false);
  header_recoverablefiles.set_alignment(Gtk::ALIGN_LEFT);
  headers_col2.pack_start(header_otherfiles);
  header_otherfiles.set_sensitive(false);
  header_otherfiles.set_alignment(Gtk::ALIGN_LEFT);
  //headers_col2.pack_start(header_blocks);
  //header_blocks.set_sensitive(false);
  //header_blocks.set_alignment(Gtk::ALIGN_LEFT);
  

  main_VBox.pack_start(main_HBox);

  // Sets the border width of the window.
  //set_border_width(10);

  // filename
  //m_filename.set_max_length(50);
  //filename_entry.set_text(text);
  //main_HBox.pack_start(filename_entry);


  // Progress bar
  main_VBox.pack_start(progressBar);
  progressBar.set_text(" ");
  main_VBox.pack_start(globalProgress);
  globalProgress.set_text(" ");
  //progressBar.set_fraction(0.5);
  //progressBar.set_text("Coucou");
  //progressBar.modify_bg(Gtk::STATE_NORMAL,Gdk::Color::Color("red"));


  // Files done
  main_VBox.pack_start(done_frame);
  done_frame.set_label(_("Scanned files"));
  done_window.add(done_files);
  done_window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  done_frame.add(done_window);

  // Tags for the latter textbuffer
  Glib::RefPtr<Gtk::TextBuffer::Tag> tagOk = 
    Gtk::TextBuffer::Tag::create("ok");
  tagOk->property_foreground() = "green";
  tagOk->property_editable() = false;

  Glib::RefPtr<Gtk::TextBuffer::Tag> tagWarning = 
    Gtk::TextBuffer::Tag::create("warning");
  tagWarning->property_foreground() = "orange";
  tagWarning->property_editable() = false;

  Glib::RefPtr<Gtk::TextBuffer::Tag> tagError = 
    Gtk::TextBuffer::Tag::create("error");
  tagError->property_foreground() = "red";
  tagError->property_editable() = false;

  Glib::RefPtr<Gtk::TextBuffer::Tag> tagEdit = 
    Gtk::TextBuffer::Tag::create("edit");
  tagError->property_editable() = false;

  Glib::RefPtr<Gtk::TextBuffer> textb = done_files.get_buffer();
  Glib::RefPtr<Gtk::TextBuffer::TagTable> refTagTable = textb->get_tag_table();
  refTagTable->add(tagOk);
  refTagTable->add(tagWarning);
  refTagTable->add(tagError);
  refTagTable->add(tagEdit);
  textb->apply_tag(tagEdit, textb->begin(), textb->end());
  
  // Status
  main_VBox.pack_start(status_frame);
  status_frame.set_label(_("Status"));
  status_frame.add(status_label);

  show_all_children();
  while(Gtk::Main::events_pending()) Gtk::Main::iteration();


  if (strcmp("",text))
    preprocess();
}

MainWindow::~MainWindow()
{
}

void MainWindow::repair() {
  verify();
  progressBar.set_text(_("Repairing..."));  
  status_label.set_text(_("Repairing..."));
  operation = repairing;
  Result result = repairer->Process(true);
  operation = none;
  update_status(result);
}

void MainWindow::verify() {
  status_label.set_text(_("Verifying..."));
  operation = verifying;
  Result result = repairer->Process(false);
  operation = none;
  update_status(result);
}

void MainWindow::preprocess()
{
  status_label.set_text(_("Scanning headers..."));
  operation = scanning;

  CommandLine *commandline = new CommandLine;

  Result result = eInvalidCommandLineArguments;
  
  char** argv;
  char* args [4];
  argv = args;
  args[0] = "par2";
  args[1] = "r";
  char* buffer = strdup(filename_entry.get_text().c_str());
  args[2] = buffer;
  //args[2] = "-q";
  //args[4] = "-q";

  bool res = commandline->Parse(3, argv);
  if (res == false) {
    status_label.set_text("Error !");
    status_frame.modify_bg(Gtk::STATE_NORMAL,Gdk::Color::Color("red"));
  }
  else {
    // Repair damaged files
    //    switch (commandline->GetVersion())
    //  {
    //  case CommandLine::verPar1:
    //{
    //  Par1Repairer *repairer = new Par1Repairer;
    //  result = repairer->Process(*commandline, true);
	  //delete repairer;
    //}
    //break;
    //case CommandLine::verPar2:
    //{
    repairer = new LibPar2(commandline);
    repairer->sig_filename.
      connect( sigc::mem_fun(*this,&MainWindow::signal_filename));
    repairer->sig_progress.
      connect( sigc::mem_fun(*this,&MainWindow::signal_progress));
    repairer->sig_headers.
      connect( sigc::mem_fun(*this,&MainWindow::signal_headers));
    repairer->sig_done.
      connect( sigc::mem_fun(*this,&MainWindow::signal_done));
    result = repairer->PreProcess();
    //delete repairer;
	  //}
	  //break;
	  //default:
	  //break;
	  //}
  }
  //delete commandline;
  
  //  update_status(result);
  nbdone = 0.0;
  free(buffer);

  globalProgress.set_text(" ");
  progressBar.set_text(" ");
  progressBar.set_fraction(0.0);
  status_label.set_text("");
  operation = none;
}

void MainWindow::signal_filename(std::string filename) {
  progressBar.set_text(filename);
  while(Gtk::Main::events_pending()) Gtk::Main::iteration();

  /*
  if (operation == repairing || operation == verifying) {
    nbdone += 1.0;
    globalProgress.set_fraction(nbdone/nbfiles);
    globalProgress.set_text(itos(int(nbdone/nbfiles*100.0)).append(" %"));
  }
  progressBar.set_text(filename);
  while(Gtk::Main::events_pending()) Gtk::Main::iteration();
  */
}

void MainWindow::signal_progress(double progress) {
  progressBar.set_fraction(progress/1000.0);
  while(Gtk::Main::events_pending()) Gtk::Main::iteration();
}

void MainWindow::signal_headers(ParHeaders* headers) {
  if (headers->setid.size() == 0)
    header_setid.set_sensitive(false);
  else {
    header_setid.set_text(ustring(_("Set ID: ")).append(headers->setid));
    header_setid.set_sensitive(true);
  }
  if (headers->block_size == -1)
    header_blocksize.set_sensitive(false);
  else {
    double size = headers->block_size;
    string str;
    if (size < 1024.0)
      str = dtos(size).append(_(" bytes"));
    else if (size < 1024.0*1024)
      str = dtos(size/1024.0).append(_(" kbytes"));
    else if (size < 1024.0*1024*1024)
      str = dtos(size/(1024.0*1024)).append(_(" Mbytes"));
    else
      str = dtos(size/(1024.0*1024*1024)).append(_(" Gbytes"));
    header_blocksize.set_text(ustring(_("Block size: ")).append(str));
    header_blocksize.set_sensitive(true);
  }
  /*if (headers->chunk_size == -1)
    header_chunksize.set_sensitive(false);
  else {
    header_chunksize.set_text(ustring(_("Chunk size: ")).
			      append(itos(headers->chunk_size)));
    header_chunksize.set_sensitive(true);
    }*/
  if (headers->data_blocks == -1)
    header_sourceblockcount.set_sensitive(false);
  else {
    header_sourceblockcount.set_text(ustring(_("Data blocks: ")).
			      append(itos(headers->data_blocks)));
    header_sourceblockcount.set_sensitive(true);
  }
  if (headers->data_size == -1)
    header_totalsize.set_sensitive(false);
  else {
    double size = headers->data_size;
    string str;
    if (size < 1024.0)
      str = dtos(size).append(_(" bytes"));
    else if (size < 1024.0*1024)
      str = dtos(size/1024.0).append(_(" kbytes"));
    else if (size < 1024.0*1024*1024)
      str = dtos(size/(1024.0*1024)).append(_(" Mbytes"));
    else
      str = dtos(size/(1024.0*1024*1024)).append(_(" Gbytes"));
    header_totalsize.set_text(ustring(_("Data size: ")).append(str));
    header_totalsize.set_sensitive(true);
  }
  if (headers->recoverable_files == -1)
    header_recoverablefiles.set_sensitive(false);
  else {
    nbfiles = headers->recoverable_files;
    header_recoverablefiles.set_text(ustring(_("Recoverable files: ")).
			      append(itos(headers->recoverable_files)));
    header_recoverablefiles.set_sensitive(true);
  }
  if (headers->other_files == -1)
    header_otherfiles.set_sensitive(false);
  else {
    header_otherfiles.set_text(ustring(_("Other files: ")).
			      append(itos(headers->other_files)));
    header_otherfiles.set_sensitive(true);
  }

  while(Gtk::Main::events_pending()) Gtk::Main::iteration();
}

void MainWindow::signal_done(std::string filename, int blocks_available, 
		 int blocks_total) {

  Glib::RefPtr<Gtk::TextBuffer> textb = done_files.get_buffer();

  Gtk::TextBuffer::iterator iter =  textb->begin();

  // Update done_window
  if (operation == scanning) {
    textb->set_text("Loaded: "+filename+"\n"+textb->get_text());
    done_files.set_buffer(textb);
  }
  else if (operation == repairing || operation == verifying) {
    if (textb->get_char_count() != 0) {
      textb->insert(iter, "\n");
      iter =  textb->begin();
    }
    if (blocks_available == blocks_total)
      textb->insert_with_tag(iter, _("Verified ")+filename+_(": ")+
			     itos(blocks_available)+"/"+itos(blocks_total)+
			     _(" blocks"),"ok");    
    else if (blocks_available == 0)
      textb->insert_with_tag(iter, _("Missing ")+filename,"error");
    else
      textb->insert_with_tag(iter, _("Corrupted ")+filename+_(": ")+
			     itos(blocks_available)+"/"+itos(blocks_total)+
			     _(" blocks"),"warning");
  }

  // update progressbar
  if (operation == verifying) {
    nbdone += 1.0;
    globalProgress.set_fraction(nbdone/nbfiles);
    globalProgress.set_text(itos(int(nbdone/nbfiles*100.0)).append(" %"));
  }
  while(Gtk::Main::events_pending()) Gtk::Main::iteration();

  //done_files.set_buffer(textb);

}

void MainWindow::update_status(Result result) {
 switch (result) {
  case eSuccess:
    status_label.set_text(_("Success !"));
    status_frame.modify_bg(Gtk::STATE_NORMAL,Gdk::Color::Color("green"));
    break;
  case eRepairPossible:
    status_label.set_text(_("Repair possible"));
    status_frame.modify_bg(Gtk::STATE_NORMAL,Gdk::Color::Color("blue"));
    break;
  case eRepairNotPossible:
    status_label.set_text(_("Repair not possible"));
    status_frame.modify_bg(Gtk::STATE_NORMAL,Gdk::Color::Color("red"));
    break;
  case eInsufficientCriticalData:
    status_label.set_text(_("Insufficient critical data"));
    status_frame.modify_bg(Gtk::STATE_NORMAL,Gdk::Color::Color("red"));
    break;
  case eRepairFailed:
    status_label.set_text(_("Repair failed"));
    status_frame.modify_bg(Gtk::STATE_NORMAL,Gdk::Color::Color("red"));
    break;
  case eFileIOError:
    status_label.set_text(_("I/O error"));
    status_frame.modify_bg(Gtk::STATE_NORMAL,Gdk::Color::Color("red"));
    break;
  case eLogicError:
    status_label.set_text(_("Internal error"));
    status_frame.modify_bg(Gtk::STATE_NORMAL,Gdk::Color::Color("red"));
    break;
 case eMemoryError:
   status_label.set_text(_("Out of memory"));
   status_frame.modify_bg(Gtk::STATE_NORMAL,Gdk::Color::Color("red"));
    break;
 default:
   break;
  }

}

void MainWindow::open()
{
  Gtk::FileChooserDialog dialog(_("Please choose a file"), Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(*this);

  //Add response buttons the the dialog:
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  //Add filters, so that only certain file types can be selected:

  Gtk::FileFilter filter_par2;
  filter_par2.set_name(_("PAR2 files"));
  filter_par2.add_mime_type("application/x-par2");
  dialog.add_filter(filter_par2);

  Gtk::FileFilter filter_any;
  filter_any.set_name(_("Any files"));
  filter_any.add_pattern("*");
  dialog.add_filter(filter_any);

  //Show the dialog and wait for a user response:
  int result = dialog.run();

  //Handle the response:
  switch(result)
  {
    case(Gtk::RESPONSE_OK):
    {
      filename_entry.set_text(dialog.get_filename());
      filename_title.set_sensitive(true);
      preprocess();
      break;
    }
    case(Gtk::RESPONSE_CANCEL):
    {
      break;
    }
    default:
    {
      break;
    }
  }
}

void MainWindow::about() {
  Gtk::MessageDialog dialog(*this, _("About GPar2"));
  dialog.set_secondary_text(_("GPar2 is a GUI for par2 recovery sets.\nIn case of problems, please report them to the SourceForge BugTracker.\n\n(c) 2005 Francois LESUEUR"));

  dialog.run();
}

void MainWindow::quit() {
  exit(0);
}

string itos(int i)	// convert int to string
{
  stringstream s;
  s << i;
  return s.str();
}
string dtos(double d)	// convert double to string
{
  stringstream s;
  s << d;
  return s.str();
}
