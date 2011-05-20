#ifndef userint_h
#define userint_h 

#include "names.h"
#include "network.h"
#include "devices.h"
#include "monitor.h"
#include <set>

using namespace std;

typedef set<char> charset;

const int maxline = 80;     // Maximum length of the command line.


class userint {
  names* nmz;              // Modules declared elsewhere
  devices* dmz;            // and shared with other parts 
  monitor* mmz;            // of the program.

  bool cmdok;              // Has the attempt to parse the
                           // current command line failed yet?
  char cmdline[maxline];   // Command line typed in by user.
  int  cmdlen;             // Length of current command line.
  int  cmdpos;             // Position of current character.
  char curch;              // Current character.
  char cmd;                // Command to be executed.
  int cyclescompleted;     // Simulation cycles completed.

  void readline (void);
  void getch (void);
  void skip (void);
  void rdcmd (char& cmd, charset valid);
  void rdnumber (int& n, int lo, int hi);
  void rdstring (namestring& n);
  void rdname (name& n);
  void rdqualname (name& prefix, name& suffix);
  void setswcmd (void);
  void runnetwork (int ncycles);
  void runcmd (void);
  void continuecmd (void);
  void setmoncmd (void);
  void zapmoncmd (void);
  void debugcmd (void);
  void helpcmd (void);

 public:
  void userinterface (void);
  /* Implements the interactive user command interface.                 */

  userint (names* names_mod, devices* devices_mod, monitor* monitor_mod);
  /* Constructor for the userint module.                                */
};

#endif /* userint_h */










