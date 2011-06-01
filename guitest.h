#ifndef guitest_h
#define guitest_h

#include <wx/wx.h>
#include "names.h"
#include "devices.h"
#include "monitor.h"
#include "network.h"

class MyApp: public wxApp
{
 public:
  bool OnInit(); // automatically called when the application starts
 private:
  names *nmz;    // pointer to the names class
  network *netz; // pointer to the network class
  devices *dmz;  // pointer to the devices class
  monitor *mmz;  // pointer to the monitor class
};

#endif /* guitest_h */
