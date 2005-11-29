#include "gpar2.h"


int main(int argc, char *argv[])
{
  /* Reset the locale variables LC_* */
  setlocale(LC_ALL, "");
  setlocale(LC_TIME, "" );
  setlocale(LC_MESSAGES, "");

  bindtextdomain( PACKAGE, LOCALEDIR);//"/usr/local/share/locale" );
  bind_textdomain_codeset( PACKAGE, getenv("LANG"));
  textdomain( PACKAGE ) ;

  void* handle = dlopen("libpar2.so", RTLD_LAZY);
    
  if (!handle) {
    cerr << "Cannot open library: " << dlerror() << '\n';
    return 1;
  }
  
  char* tgt = "";
  if (argc > 1)
    tgt = argv[1];
  
  Gtk::Main kit(argc, argv);
  
  
  MainWindow mainwindow(tgt);
  Gtk::Main::run(mainwindow);
    
  dlclose(handle);
  return 0;
}

