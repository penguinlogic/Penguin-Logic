#ifndef parser_h
#define parser_h

#include "names.h"
#include "scanner.h"
#include "network.h"
#include "devices.h"
#include "monitor.h"
#include "symbol.h"


using namespace std;

class parser {
  network* netz; // instantiations of various classes for parser to use.
  devices* dmz; 
  monitor* mmz;
  scanner* smz;

  /* put other stuff that the class uses internally here */
  /* also declare internal functions                     */
  symbol* cursym;

  void uint (void); // throws uintex if not given a valid uint
  void boolrule (void); // throws boolruleex if not given a valid bool
  void inname (void); // throws innameex if not given a valid inname
  void outname (void); // throws outnameex if not given a valid outname
  void device (void); // throws deviceex if finds invalid device
  void uname (void); // throws unameex if not given a valid uname
  void monrule (void); // throws monruleex if finds invalid monrule
  void connrule (void); // throws connruleex if finds invalid connrule
  void devrule (void); // throws devruleex if finds invalid devrule
  void section (void); // throws sectionex if finds invalid section
  void parsedeffile (void);

 public:
  bool readin ();
    /* Reads the definition of the logic system and builds the             */
    /* corresponding internal representation via calls to the 'Network'    */
    /* module and the 'Devices' module.                                    */

  parser (network* network_mod, devices* devices_mod,
	  monitor* monitor_mod, scanner* scanner_mod);
    /* the constructor takes pointers to various other classes as parameters */
};

#endif /* parser_h */

