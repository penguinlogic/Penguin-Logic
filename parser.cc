#include <iostream>
#include "parser.h"

using namespace std;

/* The parser for the circuit definition files */

/***************************************************************************************/
bool parser::readin (void)
{
  try {
    parsedeffile();
//    cout << "It worked!" << endl;
	if (errcount == 0) {
		cout << "Definition file parsed successfully" << endl;
		return true;
	}
	else {
		cout << "Could not parse definition file: " << errcount << " errors found" << endl;
		return false;
	}
  }
  catch (exception& e) {
    cout << "Cannot run simulator" << endl;
    return false;
  }
}

parser::parser (network* network_mod, devices* devices_mod,
		monitor* monitor_mod, scanner* scanner_mod, names* names_mod)
{
//cout << "parser constructor" << endl;
  netz = network_mod;  /* make internal copies of these class pointers */
  dmz = devices_mod;   /* so we can call functions from these classes  */
  mmz = monitor_mod;   /* eg. to call makeconnection from the network  */
  smz = scanner_mod;   /* class you say:                               */
                      /* netz->makeconnection (i1, i2, o1, o2, ok);   */
 nmz = names_mod;
 // cursym = *new symbol();
	symbol cursym;
	errcount = 0;
	section_var.set_parameters(Section, DEVICES, -1, -1); // Used in section()
	                                                      // to ensure correct ordering
}
/***************************************************************************************/
class parseex : public exception // thrown by catch statements to indicate failed
				                 // parsing once error has been reported
{
	public:
		virtual const char* what () const throw ()
		{
			return "Exception: failed to parse definition file";
		}
} parseex_i;
/***************************************************************************************/
class uintex : public exception
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected a positive integer";
  }
} uintex_i;
/***************************************************************************************/
void parser::uint (int& uint_var) // throws uintex if not given a valid uint
	                              // updates uint_var with given uint
{
try
  {
    if (cursym.get_type() != Uint) {errcount++; throw uintex_i;} // didn't get uint
	else uint_var = cursym.get_uint();
  }
catch(exception& e)
  {
    //cout << e.what() << endl;
	smz->print_err(e.what());
    throw;
  }
}
/***************************************************************************************/
class boolex : public exception // used in parser::device
{
  virtual const char* what () const throw ()
  {
    return "Exception: expected a Boolean value";
  }
} boolex_i;

/***************************************************************************************/
class innameex : public exception
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected an inname";
  }
} innameex_i;
/***************************************************************************************/
void parser::inname (name & iname) // throws innameex if not given a valid inname
{
try
  {
    if (cursym.get_type() != Inname) {errcount++; throw innameex_i;} // didn't get an inname
 else iname = cursym.get_name_id();
 }
 catch (exception& e)
   {
//    // cout << e.what() << endl;
//	smz->print_err(e.what());
     throw;
   }
}
/***************************************************************************************/
class outnameex : public exception
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected an outname";
  }
} outnameex_i;
/***************************************************************************************/
void parser::outname (name &oname) // throws outnameex if not given a valid outname
{
try
  {
    if (cursym.get_type() != Outname) {errcount++; throw outnameex_i;} // didn't get an outname
else oname = cursym.get_name_id();  
	}
 catch (exception& e)
   {
//   //  cout << e.what() << endl;
//	smz->print_err(e.what());
     throw;
   }
}
/***************************************************************************************/
class deviceex : public exception
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected a device";
  }
} deviceex_i;
/***************************************************************************************/
class switchex : public deviceex {};
/***************************************************************************************/
class swperiodex : public switchex
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected switch 'period'";
  }
} swperiodex_i;
/***************************************************************************************/
class swinitialvalueex : public switchex
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected switch 'initialvalue'";
  }
} swinitialvalueex_i;
/***************************************************************************************/
class swnuminputsex : public switchex
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected switch 'numinputs'";
  }
} swnuminputsex_i;
/***************************************************************************************/
class devdashex : public deviceex
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected '-'";
  }
} devdashex_i;


/******************************************************************************************************
tests device-specific syntax and, if valid, sets parameters for device creation
*/

void parser::device (devicekind &devkind_var, int &variant_var) // throws deviceex if finds invalid device
{
try
  {
    if (cursym.get_type() == Devname) { //i.e. if symbol is a 'devname'
	/* Now we need to test if the device is valid.
        We test each combination of devname, devswitch, and if
        necessary the next symbol to make sure of a valid device
	*/
	if (cursym.get_name_id() == nmz->cvtname("CLOCK")) { //i.e. devname is CLOCK
	  smz->getsymbol (cursym);
	  if (cursym.get_syntaxvalue() == dash) { // i.e. symbol is '-'
	    smz->getsymbol (cursym);
	    if (cursym.get_syntaxvalue() == period) { //i.e. devswitch is '-period'
	      smz->getsymbol (cursym);
	      if (cursym.get_type() == Uint) { // i.e. symbol is a uint
			  devkind_var = aclock;
			  variant_var = cursym.get_uint();
				  return; // we have a valid CLOCK
	      } else {errcount++; throw uintex_i;} // expected a uint
	    } else {errcount++; throw swperiodex_i;} // expected 'period'
	  } else {errcount++; throw devdashex_i;} // expected '-'
	}

	if (cursym.get_name_id() == nmz->cvtname("SWITCH")) { //i.e. devname is SWITCH
	  smz->getsymbol (cursym);
	  if (cursym.get_syntaxvalue() == dash) { // i.e. symbol is '-'
	    smz->getsymbol (cursym);
	    if (cursym.get_syntaxvalue() == initialvalue) { //i.e. devswitch is 'initialvalue'
	      smz->getsymbol (cursym);
	      if (cursym.get_type() == Uint &&
		  (cursym.get_uint() == 0 || cursym.get_uint() == 1)) { // i.e. symbol is a uint but is also a bool
			  devkind_var = aswitch;
			  variant_var = cursym.get_uint();
			  return; // we have a valid SWITCH
	      } else {errcount++; throw boolex_i;} // expected a bl
	    } else {errcount++; throw swinitialvalueex_i;}// expected '-initialvalue'
      	  } else {errcount++; throw devdashex_i;} // expected '-'
	}

	//TODO: check uint within bounds (1-16) for all these devices
	if (cursym.get_name_id() == nmz->cvtname("AND")) { //i.e. devname is AND
	  smz->getsymbol (cursym);
	  if (cursym.get_syntaxvalue() == dash) { // i.e. symbol is '-'
	    smz->getsymbol (cursym);
	    if (cursym.get_syntaxvalue() == numinputs) { //i.e. devswitch is 'numinputs'
	      smz->getsymbol (cursym);
	      if (cursym.get_type() == Uint) { // i.e. symbol is a uint
			  devkind_var = andgate;
			  variant_var = cursym.get_uint();
			  return; // we have a valid AND
	      } else {errcount++; throw uintex_i;} // expected a uint
	    } else {errcount++; throw swnuminputsex_i;} // expected 'numinputs'
	  } else {errcount++; throw devdashex_i;} // expected '-'

	}

	if (cursym.get_name_id() == nmz->cvtname("NAND")) { //i.e. devname is NAND
	  smz->getsymbol (cursym);
	  if (cursym.get_syntaxvalue() == dash) { // i.e. symbol is '-'
	    smz->getsymbol (cursym);
	    if (cursym.get_syntaxvalue() == numinputs) { //i.e. devswitch is 'numinputs'
	      smz->getsymbol (cursym);
	      if (cursym.get_type() == Uint) { // i.e. symbol is a uint
			  devkind_var = nandgate;
			  variant_var = cursym.get_uint();
		return; // we have a valid NAND
	      } else {errcount++; throw uintex_i;} // expected a uint
	    } else {errcount++; throw swnuminputsex_i;} // expected 'numinputs'
	  } else {errcount++; throw devdashex_i;} // expected '-'
	}

	if (cursym.get_name_id() == nmz->cvtname("OR")) { //i.e. devname is OR
	  smz->getsymbol (cursym);
	  if (cursym.get_syntaxvalue() == dash) { // i.e. symbol is '-'
	    smz->getsymbol (cursym);
	    if (cursym.get_syntaxvalue() == numinputs) { //i.e. devswitch is '-numinputs'
	      smz->getsymbol (cursym);
	      if (cursym.get_type() == Uint) { // i.e. symbol is a uint
			  devkind_var = orgate;
			  variant_var = cursym.get_uint();
		return; // we have a valid OR
	      } else {errcount++; throw uintex_i;} // expected a uint
	    } else {errcount++; throw swnuminputsex_i;} // expected 'numinputs'
	  } else {errcount++; throw devdashex_i;} // expected '-'
	}

	if (cursym.get_name_id() == nmz->cvtname("NOR")) { //i.e. devname is NOR
	  smz->getsymbol (cursym);
	  if (cursym.get_syntaxvalue() == dash) { // i.e. symbol is '-'
	    smz->getsymbol (cursym);
	    if (cursym.get_syntaxvalue() == numinputs) { //i.e. devswitch is '-numinputs'
	      smz->getsymbol (cursym);
	      if (cursym.get_type() == Uint) { // i.e. symbol is a uint
			  devkind_var = norgate;
			  variant_var = cursym.get_uint();
		return; // we have a valid NOR
	      } else {errcount++; throw uintex_i;} // expected a uint
	    } else {errcount++; throw swnuminputsex_i;} // expected 'numinputs'
	  } else {errcount++; throw devdashex_i;} // expected '-'
	}

	if (cursym.get_name_id() == nmz->cvtname("DTYPE")) { //i.e. devname is DTYPE
		devkind_var = dtype;
	  return; // we have a valid DTYPE
	}

	if (cursym.get_name_id() == nmz->cvtname("XOR")) { //i.e. devname is XOR
		devkind_var = xorgate;
	  return; // we have a valid XOR
	}

	throw deviceex_i; // didn't get a valid devname
    } else {errcount++; throw deviceex_i;} // didn't get a devname to start with
  }
 catch (exception& e)
   {
//     //cout << e.what() << endl;
//     smz->print_err(e.what());
     throw;
   }
}

/***************************************************************************************/
class undefunameex : public exception
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: device not defined in DEVICES";
  }
} undefunameex_i;


/**************************************************************************************/
class unameex : public exception
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected a user-defined name, as assigned in DEVICES";
  }
} unameex_i;

/**************************************************************************************/
void parser::uname (name &did_var) // throws unameex if not given a valid uname
{
try
  {
    if (cursym.get_type() != Uname) {errcount++; throw unameex_i;} // didn't get a uname
	if (DEVICES != section_var.get_syntaxvalue()) {
			for(int i = 0; i < uname_list.size(); i++) {
				if (cursym.get_name_id() == uname_list[i]) {did_var = cursym.get_name_id(); return;}
			}
			errcount++;
			throw undefunameex_i; // we get here if we're in CONN. or MON. and find a uname
			                      // not defined in DEVICES
			}
	else did_var = cursym.get_name_id();
  }
 catch (exception& e)
   {
//     //cout << e.what() << endl;
//	smz->print_err(e.what());
     throw;
   }
}

/***************************************************************************************/
class rulesymbolex : public exception {};

class rulesymequalsex : public rulesymbolex
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected an '='";
  }
} rulesymequalsex_i;

/***************************************************************************************/
class rulesymrarrowex : public rulesymbolex
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected a '>'";
  }
} rulesymrarrowex_i;

/***************************************************************************************/
class rulesymsemicolonex : public rulesymbolex
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected a ';'";
  }
} rulesymsemicolonex_i;

/***************************************************************************************/
class rulesymdotex : public rulesymbolex
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected a '.'";
  }
} rulesymdotex_i;

/***************************************************************************************/
class makemonex : public exception
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: Error setting monitor point";
  }
} makemonex_i;

/***************************************************************************************/
void parser::monrule (void) // throws monruleex if finds invalid monrule
{
try
  {
	name dev;
	name outp=blankname;
	bool ok;
    uname(dev);
    smz->getsymbol (cursym);
    if (cursym.get_syntaxvalue() == dot) { // cursym is '.'
      smz->getsymbol (cursym);
      outname(outp);
	  smz->getsymbol (cursym);
    } 
	if (cursym.get_syntaxvalue() == semicolon) { // cursym is ';'
	// checks to make sure unames have been defined in DEVICES
	int valid_dev_count = 0;
	for (int i = 0; i < uname_list.size(); i++) {
		if (dev == uname_list[i]) valid_dev_count++;
	}
	if (0 == valid_dev_count) {errcount++; throw undefunameex_i;}//we found a device not initialised in DEVICES
    if (0 == errcount) {
	mmz->makemonitor(dev, outp, ok);      
	if (ok){
	cout<<"Monitor set at ";
	nmz->writename(dev);
	if (outp !=blankname) {cout<<"."; nmz->writename(outp);}
	cout<<endl;
	return;
	}else {errcount++; throw makemonex_i;}
	}
    } else {errcount++; throw rulesymsemicolonex_i;} // didn't get a '.' or ';'
  }
 catch (exception& e) // Only catches an error in monrule syntax. Does not
                      // throw it further, so subsequent lines can contine
					  // to be parsed
   {
	smz->print_err(e.what());
     while (cursym.get_syntaxvalue() != semicolon && cursym.get_syntaxvalue() != endfile) { // iterates while cursym is not ';'
       smz->getsymbol (cursym);
	   //throw parseex_i;
     }
   }
}

/***************************************************************************************/
class makeconex : public exception
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: Connection failed - input pin/device mismatch";
  }
} makeconex_i;

/***************************************************************************************/
class badoutpex : public exception
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: Output pin name not valid for given device";
  }
} badoutpex_i;
/***************************************************************************************/
class idevalreadyusedex : public exception
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: Input pin already connected";
  }
} idevalreadyusedex_i;
/***************************************************************************************/

void parser::connrule (void) // throws connruleex if finds invalid connrule
{
try
  {
    name odev, idev, inp;
    name outp=blankname;
	bool ok;
    uname(odev);
    smz->getsymbol (cursym);
    if (cursym.get_syntaxvalue() == dot) { // cursym is '.'
    smz->getsymbol (cursym);
    outname(outp);
	if (outp != blankname && dtype != netz->finddevice(odev)->kind) {errcount++; throw badoutpex_i;}
	// (above) we got an output of Q or QBAR with a non-DTYPE device
	smz->getsymbol(cursym);
    }
	if (cursym.get_syntaxvalue() == rarrow) { // cursym is '>'
    smz->getsymbol (cursym);
    uname(idev);
    smz->getsymbol (cursym);
    if (cursym.get_syntaxvalue() == dot) { // cursym is '.'
      smz->getsymbol (cursym);
      inname(inp);
	  smz->getsymbol (cursym);
      if (cursym.get_syntaxvalue() == semicolon) { // cursym is ';'
	for (int i = 0; i < idev_conn_list.size(); i++) {
		if (idev == idev_conn_list[i] && inp == inp_conn_list[i]) {errcount++; throw idevalreadyusedex_i;}
	}

	if (0 == errcount) {
	netz->makeconnection (idev, inp, odev, outp, ok);
	if (ok){
	idev_conn_list.push_back(idev); // append uname id of input device once successfully connected
	inp_conn_list.push_back(inp); // append uname id of input pin once successfully connected
	                           // we can now check that future connections do not connect
							   // input pins that have already been used
	cout<<"Connection made between ";
	nmz->writename(odev);
	if (outp !=blankname) {cout<<"."; nmz->writename(outp);}
	cout<<" and ";
	nmz->writename(idev); cout<<"."; nmz->writename(inp);
	cout<<endl;
	return;
	
	}else { //connection error
		errcount++;
		throw makeconex_i;
	}
	}
    } else {errcount++; throw rulesymsemicolonex_i;} // we expected a ';'
    } else {errcount++; throw rulesymdotex_i;} // we expected a '.'
    } else {errcount++; throw rulesymrarrowex_i;} // we expected a '>'
  }
 catch (exception& e)
   {
     //cout << e.what() << endl;
	smz->print_err(e.what());
     while (cursym.get_syntaxvalue() != semicolon && cursym.get_syntaxvalue() != endfile) { // iterates while cursym is not ';'
       smz->getsymbol (cursym);
     }
    //throw parseex_i;
   }
}

/***************************************************************************************/
class makedevex : public exception
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: Error constructing device";
  }
} makedevex_i;
/***************************************************************************************/
class dupunameex : public exception
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: Duplicate device name. Subsequent devices will not be created";
  }
} dupunameex_i;
/***************************************************************************************/
void parser::devrule (void) // throws devruleex if finds invalid devrule
{
try
  {
    devicekind devkind_var;
    int variant_var;	
    name did_var;
    bool ok;
    device(devkind_var, variant_var);
    smz->getsymbol (cursym);
    if (cursym.get_syntaxvalue() == equals) { // we have an '='
	smz->getsymbol (cursym);     
	uname(did_var);
	for(int i = 0; i < uname_list.size(); i++) {
	if (did_var == uname_list[i]) {errcount++; throw dupunameex_i;}	// we found a duplicate uname
	}

	smz->getsymbol (cursym);
      if (cursym.get_syntaxvalue() == semicolon) { // we have a ';'
	if (0 == errcount) {
	dmz->makedevice(devkind_var, did_var, variant_var, ok); 
	uname_list.push_back(did_var); // adds uname id to uname_list vector so we can check elsewhere
	                               // for duplicate/undefined unames
	if (ok){
	dmz->writedevice(devkind_var);
	cout<<": ";	
	nmz->writename(did_var);
	cout<<" made"<<endl;	
	return;
	}	
	else {errcount++; throw makedevex_i;}	//could not construct device
	}
	} else {errcount++; throw rulesymsemicolonex_i;} // we expected a ';'
    } else {errcount++; throw rulesymequalsex_i;} // we expected an '='
  }
 catch (exception& e)
   {
    // cout << e.what() << endl;
	smz->print_err(e.what());
     while (cursym.get_syntaxvalue() != semicolon && cursym.get_syntaxvalue() != endfile) { // iterates while cursym is not ';'
       smz->getsymbol (cursym);
     }
    //throw parseex_i;
   }
}
/***************************************************************************************/
class sectionex : public exception
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: invalid Section in definition file";
  }
} sectionex_i;
/***************************************************************************************/
class lbrakex : public sectionex
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected '{' at beginning of Section";
  }
} lbrakex_i;
/***************************************************************************************/
class sectionorderex : public sectionex
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: Sections in wrong order";
  }
} sectionorderex_i;
/***************************************************************************************/
class rbrakex : public sectionex
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected '}' at beginning of Section";
  }
} rbrakex_i;
/***************************************************************************************/
void parser::section (void) // throws sectionex if finds invalid section
{
try
  {
    smz->getsymbol (cursym);
     if (cursym.get_syntaxvalue() == endfile) return;
     if (cursym.get_syntaxvalue() == DEVICES) { // cursym is 'DEVICES'
		 if (DEVICES != section_var.get_syntaxvalue()) {errcount++; throw sectionorderex_i;} // check DEVICES comes first
	smz->getsymbol (cursym);
      if (cursym.get_syntaxvalue() == lbrak) { // cursym is '{'	
	smz->getsymbol (cursym);
	while (cursym.get_syntaxvalue() != rbrak && cursym.get_syntaxvalue() != endfile) { // i.e. while inside braces
	  devrule();
	  smz->getsymbol (cursym);
	}
      } else {errcount++; throw lbrakex_i;} // we expected an '{'
		section_var.set_parameters(Section, CONNECTIONS, -1, -1); // increment section_var ready for next section

    } else if (cursym.get_syntaxvalue() == CONNECTIONS) { // cursym is 'CONNECTIONS'
		 if (CONNECTIONS != section_var.get_syntaxvalue()) {errcount++; throw sectionorderex_i;} // check CONNECTIONS comes second
      smz->getsymbol (cursym);
      if (cursym.get_syntaxvalue() == lbrak) { // cursym is '{'
	smz->getsymbol (cursym);
	while (cursym.get_syntaxvalue() != rbrak && cursym.get_syntaxvalue() != endfile) { // i.e. while inside braces
	  connrule();
	  smz->getsymbol (cursym);
	}
      } else {errcount++; throw lbrakex_i;} // we expected an '{'
		section_var.set_parameters(Section, MONITOR, -1, -1); // increment section_var ready for next section

    } else if (cursym.get_syntaxvalue() == MONITOR) { // cursym is 'MONITOR'
		 if (MONITOR != section_var.get_syntaxvalue()) {errcount++; throw sectionorderex_i;} // check MONITOR comes last 
      smz->getsymbol (cursym);
      if (cursym.get_syntaxvalue() == lbrak) { // cursym is '{'
	smz->getsymbol (cursym);
	while (cursym.get_syntaxvalue() != rbrak && cursym.get_syntaxvalue() != endfile) { // i.e. while inside braces
	  monrule();
	 // cout << "monrule ran" << endl;
	  smz->getsymbol (cursym);
	}
      } else {errcount++; throw lbrakex_i;} // we expected an '{'
		section_var.set_parameters(Section, novalue, -1, -1); // set section_var to null now we've done all sections
    } else {errcount++; throw sectionex_i;} // didn't get a valid section keyword
  }
catch (sectionex& e)
   {
     //cout << e.what() << endl;
	smz->print_err(e.what());
	while (!(cursym.get_syntaxvalue() == rbrak)) { // iterates while cursym is not '}'
       smz->getsymbol (cursym);
     }
     //throw parseex_i;
   }
}
/***************************************************************************************/
class checknetworkex : public exception
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: Some inputs not connected to outputs";
  }
} checknetworkex_i;

/***************************************************************************************/
void parser::parsedeffile (void)
{
	try {
  do {
   section();
  } while (!(cursym.get_syntaxvalue() == endfile)); // i.e. while not eof
  bool ok;
  netz->checknetwork(ok);
  if (!ok){throw checknetworkex_i;}

}
catch (exception& e) {
	cout << "Parse error: " << e.what() << endl;
	throw;
}
}
