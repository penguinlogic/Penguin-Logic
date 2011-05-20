#include <iostream>
#include "userint.h"
#include <cctype>

using namespace std;

/* User interface specification */


/***********************************************************************
 *
 * Print a # prompt and read in a line from the keyboard.
 *
 */
void userint::readline (void)
{
  char ch;
  do {
    cout << "# " << flush;
    cin.getline (cmdline, maxline);
    cmdlen = cin.gcount() - 1;
  } while (cmdlen == 0);
  cmdpos = 0;
  curch = cmdline[cmdpos];
}


/***********************************************************************
 *
 * Select the next character for processing.
 *
 */
void userint::getch (void)
{
  cmdpos++;
  if (cmdpos < cmdlen)
    curch = cmdline[cmdpos];
  else
    curch = '\0';
}


/***********************************************************************
 *
 * Skip white space.
 *
 */
void userint::skip (void)
{
  while ((curch == ' ') && (cmdpos < (cmdlen - 1)))
    getch ();
}


/***********************************************************************
 *
 * Read a character from input text string.
 * Print an error if it is not a valid command.
 *
 */
void userint::rdcmd (char& cmd, charset valid)
{
  skip ();
  cmdok = false;
  if (curch != ' ') {
    if (valid.count(curch) > 0) {
      cmdok = true;
      cmd = curch;
      getch ();
    } else
      cout << "Illegal command - try 'h' for help" << endl;
  }
}


/***********************************************************************
 *
 * Read a number from the input text string.
 *
 */
void userint::rdnumber (int& n, int lo, int hi)
{
  skip ();
  n = 0;
  cmdok = isdigit(curch);
  if (cmdok) {
    do {
      n = n * 10 + ((int) curch) - ((int) '0');
      getch ();
    } while (isdigit (curch));
    if ((n < lo) || (n > hi)) {
      cmdok = false;
      cout << "Error: number out of range" << endl;
    }
  } else
    cout << "Error: number wanted" << endl;
}


/***********************************************************************
 *
 * Read the next token from the input text string.
 *
 */
void userint::rdstring (namestring &n)
{
  int i = 0;
  skip ();
  cmdok = isalpha(curch);
  if (cmdok) {
    do {
      if (i < maxlength) {
	n += curch;
	i++;
      }
      getch ();
    } while (isdigit(curch) || isalpha(curch));
  } else
    cout << "Error: name wanted" << endl;
}


/***********************************************************************
 *
 * Read a name from the input text string.
 *
 */
void userint::rdname (name& n)
{
  namestring ns;
  rdstring (ns);
  if (cmdok) {
    n = nmz->cvtname (ns);
    if (n == blankname) {
      cout << "Error: unknown name" << endl;
      cmdok = false;
    }
  }
}


/***********************************************************************
 *
 * Read a qualified name from the input text string.
 *
 */
void userint::rdqualname (name& prefix, name& suffix)
{
  rdname (prefix);
  if (cmdok) {
    skip ();
    if (curch == '.') {
      getch ();
      rdname (suffix);
    } else 
      suffix = blankname;
  }
}


/***********************************************************************
 *
 * The 's' command.
 * Sets a specified switch either on or off.
 *
 */
void userint::setswcmd (void)
{
  name swid;
  int level;
  rdname (swid);
  if (cmdok) {
    rdnumber (level, 0, 1);
    if (cmdok) {
      if (level == 0) 
	dmz->setswitch (swid, low, cmdok);
      else
	dmz->setswitch (swid, high, cmdok);
      if (! cmdok)
	cout << "Error: unknown switch" << endl;
    }
  }
}


/***********************************************************************
 *
 * Actually execute the network.
 * This is used by runcmd and continuecmd.
 *
 */
void userint::runnetwork (int ncycles)
{
  bool ok = true;
  int n = ncycles;
  while ((n > 0) && ok) {
    dmz->executedevices (ok);
    if (ok) {
      n--;
      mmz->recordsignals ();
    } else
      cout << "Error: network is oscillating" << endl;
  }
  if (ok) {
    mmz->displaysignals ();
    cyclescompleted = cyclescompleted + ncycles;
  } else
    cyclescompleted = 0;
}


/***********************************************************************
 *
 * The 'r' command.
 * Runs the simulation for a specified number of cycles.
 *
 */
void userint::runcmd (void)
{
  int n;
  int ncycles;
  cyclescompleted = 0;
  rdnumber (ncycles, 1, maxcycles);
  if (cmdok) {
    mmz->resetmonitor ();
    cout << "Running for " << ncycles << " cycles" << endl;
    runnetwork(ncycles);
  }
}


/***********************************************************************
 *
 * The 'c' command.
 * Continues the simulation for a specified number of cycles.
 *
 */
void userint::continuecmd (void)
{
  int ncycles;
  rdnumber (ncycles, 1, maxcycles);
  if (cmdok)
    if (cyclescompleted > 0) {
      if ((ncycles + cyclescompleted) > maxcycles)
	ncycles = maxcycles - cyclescompleted;
      cout << "Continuing for " << ncycles << " cycles" << endl;
      runnetwork (ncycles);
    } else
      cout << "Error: nothing to continue!" << endl;
}


/***********************************************************************
 *
 * The 'm' command.
 * Adds a monitor to a signal.
 *
 */
void userint::setmoncmd (void)
{
  name dev, outp;
  rdqualname (dev, outp);
  mmz->makemonitor (dev, outp, cmdok);
  if (cmdok)
    cyclescompleted = 0;
  else
    cout << "Error: unable to set monitor point" << endl;
}


/***********************************************************************
 *
 * The 'z' command.
 * Removes monitor from a signal.
 *
 */
void userint::zapmoncmd (void)
{
  name dev, outp;
  rdqualname (dev, outp);
  if (cmdok) {
    mmz->remmonitor (dev, outp, cmdok);
    if (cmdok)
      cyclescompleted = 0;
    else
      cout << "Error: unable to zap monitor point" << endl;
  }
}


/***********************************************************************
 *
 * The 'd' command.
 * Turns on and off debugging in the devices module.
 *
 */
void userint::debugcmd (void)
{
  int n;
  rdnumber (n, 0, 1);
  if (cmdok) {
    bool dbs = (n == 1);
    dmz->debug (dbs);
    cout << "Debugging is " << (dbs ? "on" : "off") << endl;
  }
}


/***********************************************************************
 *
 * The 'h' command.
 * Gives help on the other commands available.
 *
 */
void userint::helpcmd (void)
{
  cout << endl;
  cout << "User commands -" << endl;
  cout << endl;
  cout << "r N       - run the simulation for N cycles" << endl;
  cout << "c N       - continue simulation for N cycles" << endl;
  cout << "s X N     - set switch X to N (0 or 1)" << endl;
  cout << "m X       - set a monitor on signal X" << endl;
  cout << "z X       - zap the monitor on signal X" << endl;
  cout << "d N       - set debugging on (N=1) or off (N=0)" << endl;
  cout << "h         - help (this command)" << endl;
  cout << "q         - quit the program" << endl;
  cout << endl;
}


/***********************************************************************
 *
 * Implements the interactive user command interface.
 * The user types single letter commands and they get executed.
 *
 */
void userint::userinterface (void)
{
  cout << "Logic Simulator: interactive command interface" << endl;
  cyclescompleted = 0;
  do {
    readline ();
    /* The next two lines create a 'set' of characters which are */
    /* characters that can form valid commands.                  */
    /* See the standard templates library for more information.  */
    char poscm[] = {'s','r','c','d','z','m','h','q'};
    charset cmset(poscm, poscm + 8);
    rdcmd (cmd, cmset);
    if (cmdok) 
      switch (cmd) {
      case 's': setswcmd ();    break;
      case 'r': runcmd ();      break;
      case 'c': continuecmd (); break;
      case 'm': setmoncmd ();   break;
      case 'z': zapmoncmd ();   break;
      case 'd': debugcmd ();    break;
      case 'h': helpcmd ();     break;
      case 'q':                 break;
      }
  } while (cmd != 'q');
  cout << "Logic Simulator: terminating." << endl;
}


/***********************************************************************
 *
 * Constructor for userint module.
 * Remember the names of the other shared modules that we use here.
 *
 */
userint::userint (names* names_mod, devices* devices_mod, monitor* monitor_mod)
{
  nmz = names_mod;
  dmz = devices_mod;
  mmz = monitor_mod;
}









