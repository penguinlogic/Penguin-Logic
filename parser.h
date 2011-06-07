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
 private:
    network * netz;		// instantiations of various classes for parser to use.
    devices *dmz;
    monitor *mmz;
    scanner *smz;
    names *nmz;
    /* put other stuff that the class uses internally here */
    /* also declare internal functions                     */
    symbol cursym;
    int errcount;
    symbol section_var;		// tracks which Section (DEVICES, CONNECTION, MONITOR) we
    // are in for semantic error checking
     vector < name > uname_list;	//holds a list of device unames declared in DEVICES
    // for various error checking
     vector < name > idev_conn_list;	//holds a list of device unames which have been connected
    //to avoid duplicate input connections
     vector < name > inp_conn_list;	//holds a list of input pin unames to be used in conjunction with
    // the above

    void uint(int &uint_var);	// throws uintex if not given a valid uint
    void inname(name & iname);	// throws innameex if not given a valid inname
    void outname(name & oname);	// throws outnameex if not given a valid outname
    void device(devicekind & devkind_var, int &variant_var);	// throws deviceex if finds invalid device
    void uname(name & did_var);	// throws unameex if not given a valid uname
    void monrule(void);		// throws monruleex if finds invalid monrule
    void connrule(void);	// throws connruleex if finds invalid connrule
    void devrule(void);		// throws devruleex if finds invalid devrule
    void section(void);		// throws sectionex if finds invalid section
    void parsedeffile(void);

 public:
     bool readin();
    /* Reads the definition of the logic system and builds the             */
    /* corresponding internal representation via calls to the 'Network'    */
    /* module and the 'Devices' module.                                    */

     parser(network * network_mod, devices * devices_mod,
	    monitor * monitor_mod, scanner * scanner_mod, names * names_mod);
    /* the constructor takes pointers to various other classes as parameters */
};

#endif				/* parser_h */
