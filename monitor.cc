#include <iostream>
#include "monitor.h"

using namespace std;

/***********************************************************************
 *
 * Sets a monitor on the 'outp' output of device 'dev' by placing an   
 * entry in the monitor table. 'ok' is set true if operation succeeds. 
 *
 */
void monitor::makemonitor (name dev, name outp, bool& ok)
{
  devlink d;
  outplink o;
  ok = (mtab.used < maxmonitors);
  if (ok) {
    d = netz->finddevice (dev);
    ok = (d != NULL);
    if (ok) {
      o = netz->findoutput (d, outp);
      ok = (o != NULL);
      if (ok) {
	mtab.sigs[mtab.used].devid = dev;
	mtab.sigs[mtab.used].op = o;
	(mtab.used)++;
      }
    }
  }
}


/***********************************************************************
 *
 * Removes the monitor set the 'outp' output of device 'dev'. 'ok' is  
 * set true if operation succeeds.                                     
 *
 */
void monitor::remmonitor (name dev, name outp, bool& ok)
{
  int i, j;
  bool found;
  ok = (mtab.used > 0);
  if (ok) {
    found = false;
    for (i = 0; ((i < mtab.used) && (! found)); i++)
      found = ((mtab.sigs[i].devid == dev) &&
	       (mtab.sigs[i].op->id == outp));
    ok = found;
    if (found) {
      (mtab.used)--;
      for(j = (i-1); j < mtab.used; j++)
	mtab.sigs[j] = mtab.sigs[j + 1];
    }
  }
}


/***********************************************************************
 *
 * Returns number of signals currently monitored.  
 *
 */
int monitor::moncount (void)
{
  return (mtab.used);
}


/***********************************************************************
 *
 * Returns signal level of n'th monitor point. 
 *
 */
asignal monitor::getmonsignal (int n)
{
  return (mtab.sigs[n].op->sig);
}


/***********************************************************************
 *
 * Returns name of n'th monitor. 
 *
 */
void monitor::getmonname (int n, name& dev, name& outp)
{
  dev = mtab.sigs[n].devid;
  outp = mtab.sigs[n].op->id;
}


/***********************************************************************
 *
 * Initialises monitor memory in preparation for a new output sequence.
 *
 */
void monitor::resetmonitor (void)
{
  cycles = 0;
}


/***********************************************************************
 *
 * Called every clock cycle to record the state of each monitored     
 * signal.                                                            
 *
 */
void monitor::recordsignals (void)
{
  int n;
  for (n = 0; n < moncount (); n++)
    disp[n][cycles] = getmonsignal(n);
  cycles++;
}

/***********************************************************************
 *
 * Access recorded signal trace, returns false if invalid monitor 
 * or cycle.
 *
 */
bool monitor::getsignaltrace(int m, int c, asignal &s)
{
  if ((c < cycles) && (m < moncount ())) {
    s = disp[m][c];
    return true;
  }
  return false;
}

/***********************************************************************
 *
 * Displays state of monitored signals.  
 *
 */
void monitor::displaysignals (void)
{
  const int margin = 20;
  int n, i;
  name dev, outp;
  int namesize;
  for (n = 0; n < moncount (); n++) {
    getmonname (n, dev, outp);
    namesize = nmz->namelength (dev);
    nmz->writename (dev);
    if (outp != blankname) {
      cout << ".";
      nmz->writename (outp);
      namesize = namesize + nmz->namelength (outp) + 1;
    }
    if ((margin - namesize) > 0) {
      for (i = 0; i < (margin - namesize - 1); i++)
	cout << " ";
      cout << ":";
    }
    for (i = 0; i < cycles; i++) 
      switch (disp[n][i]) {
        case high:    cout << "-"; break;
        case low:     cout << "_"; break;
        case rising:  cout << "/"; break;
        case falling: cout << "\\"; break;
      }
    cout << endl;
  }
}


/***********************************************************************
 *
 * Called to initialise the monitor module.  
 * Remember the names of the shared names and network modules.
 *
 */
monitor::monitor (names* names_mod, network* network_mod)
{
  nmz = names_mod;
  netz = network_mod;
  mtab.used = 0;
}






