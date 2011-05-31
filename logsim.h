#ifndef logsim_h
#define logsim_h

#include <wx/wx.h>
#include "names.h"
#include "devices.h"
#include "monitor.h"
#include "network.h"
#include "parser.h"
#include "wx/help.h" // Required for help file
#include "wx/fs_zip.h"	// Required for help file

class MyApp: public wxApp
{
 public:
  bool OnInit(); // automatically called when the application starts
 private:
  names *nmz;    // pointer to the names class
  network *netz; // pointer to the network class
  devices *dmz;  // pointer to the devices class
  monitor *mmz;  // pointer to the monitor class
  scanner *smz;  // pointer to the scanner class
  parser *pmz;   // pointer to the parser class
  wxHelpController *m_helpController; // pointer to the help file
};

#endif /* logsim_h */
