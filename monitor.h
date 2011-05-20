#ifndef monitor_h
#define monitor_h

#include "names.h"
#include "network.h"
#include "devices.h"

const int maxmonitors = 10;      /* max number of monitor points */
const int maxcycles = 50;        /* max number of cycles per run */

struct moninfo {
  name devid;
  outplink op;
};
struct monitortable {
  int used;
  moninfo sigs[maxmonitors];
};
typedef asignal signaltrace[maxcycles];

class monitor {
  names*   nmz;     // version of names class to use.
  network* netz;    // version of the network class to use.

  monitortable mtab;                 // table of monitored signals
  int cycles;                        // counts clock cycles
  signaltrace disp[maxmonitors]; 

 public:
  void makemonitor (name dev, name outp, bool& ok);
    /* Sets a monitor on the 'outp' output of device 'dev' by placing an   */
    /* entry in the monitor table. 'ok' is set true if operation succeeds. */
 
  void remmonitor (name dev, name outp, bool& ok);
    /* Removes the monitor set the 'outp' output of device 'dev'. 'ok' is  */
    /* set true if operation succeeds.                                     */

  int moncount (void);
    /* Returns number of signals currently monitored.                      */
 
  asignal getmonsignal (int n);
    /* Returns signal level of n'th monitor point.                         */

  bool getsignaltrace(int m, int c, asignal &s);
    /* Access recorded signal trace, returns false if invalid monitor      */
    /* or cycle                                                            */

  void getmonname (int n, name& dev, name& outp);
    /* Returns name of n'th monitor.                                       */

  void resetmonitor (void);
    /* Initialises monitor memory in preparation for a new output sequence */
 
  void recordsignals (void);
    /* Called every clock cycle to record the state of each monitored      */
    /* signal.                                                             */
 
  void displaysignals (void);
    /* Displays state of monitored signals.                                */
 
  monitor (names* names_mod, network* network_mod);
    /* Called to initialise the monitor module.                            */
};

#endif /* monitor_h */

