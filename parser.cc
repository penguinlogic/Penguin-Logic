#include <iostream>
#include "parser.h"

using namespace std;

/* The parser for the circuit definition files */


bool parser::readin (void)
{
//  try {
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
//  }
//  catch (exception& e) {
//    cout << "Failed to correctly parse definition file." << endl;
//    return false;
//  }
}

parser::parser (/*network* network_mod, devices* devices_mod,
		monitor* monitor_mod,*/ scanner* scanner_mod)
{
//cout << "parser constructor" << endl;
  //netz = network_mod;  /* make internal copies of these class pointers */
  //dmz = devices_mod;   /* so we can call functions from these classes  */
  //mmz = monitor_mod;   /* eg. to call makeconnection from the network  */
  smz = scanner_mod;   /* class you say:                               */
                       /* netz->makeconnection (i1, i2, o1, o2, ok);   */

 // cursym = *new symbol();
	symbol cursym;
	errcount = 0;

}

class parseex : public exception // thrown by catch statements to indicate failed
				                 // parsing once error has been reported
{
	public:
		virtual const char* what () const throw ()
		{
			return "Exception: failed to parse definition file";
		}
} parseex_i;

class uintex : public exception
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected a positive integer";
  }
} uintex_i;

void parser::uint (void) // throws uintex if not given a valid uint
{
try
  {
    if (cursym.get_type() != Uint) {errcount++; throw uintex_i;} // didn't get uint
  }
catch(exception& e)
  {
    //cout << e.what() << endl;
	smz->print_err(e.what());
    throw;
  }
}

class boolex : public exception // used in parser::device
{
  virtual const char* what () const throw ()
  {
    return "Exception: expected a Boolean value";
  }
} boolex_i;


class innameex : public exception
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected an inname";
  }
} innameex_i;

void parser::inname (void) // throws innameex if not given a valid inname
{
try
  {
    if (cursym.get_type() != Inname) {errcount++; throw innameex_i;} // didn't get an inname
  }
 catch (exception& e)
   {
//    // cout << e.what() << endl;
//	smz->print_err(e.what());
     throw;
   }
}

class outnameex : public exception
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected an outname";
  }
} outnameex_i;

void parser::outname (void) // throws outnameex if not given a valid outname
{
try
  {
    if (cursym.get_type() != Outname) {errcount++; throw outnameex_i;} // didn't get an outname
  }
 catch (exception& e)
   {
//   //  cout << e.what() << endl;
//	smz->print_err(e.what());
     throw;
   }
}

class deviceex : public exception
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected a device";
  }
} deviceex_i;

class switchex : public deviceex {};

class swperiodex : public switchex
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected switch 'period'";
  }
} swperiodex_i;

class swinitialvalueex : public switchex
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected switch 'initialvalue'";
  }
} swinitialvalueex_i;

class swnuminputsex : public switchex
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected switch 'numinputs'";
  }
} swnuminputsex_i;

class devdashex : public deviceex
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected '-'";
  }
} devdashex_i;

void parser::device (void) // throws deviceex if finds invalid device
{
try
  {
    if (cursym.get_type() == Devname) { //i.e. if symbol is a 'devname'
	/* Now we need to test if the device is valid.
        We test each combination of devname, devswitch, and if
        necessary the next symbol to make sure of a valid device
	*/
	if (cursym.get_value() == CLOCK) { //i.e. devname is CLOCK
	  smz->getsymbol (cursym);
	  if (cursym.get_value() == dash) { // i.e. symbol is '-'
	    smz->getsymbol (cursym);
	    if (cursym.get_value() == period) { //i.e. devswitch is '-period'
	      smz->getsymbol (cursym);
	      if (cursym.get_type() == Uint) { // i.e. symbol is a uint
		return; // we have a valid CLOCK
	      } else {errcount++; throw uintex_i;} // expected a uint
	    } else {errcount++; throw swperiodex_i;} // expected 'period'
	  } else {errcount++; throw devdashex_i;} // expected '-'
	}

	if (cursym.get_value() == SWITCH) { //i.e. devname is SWITCH
	  smz->getsymbol (cursym);
	  if (cursym.get_value() == dash) { // i.e. symbol is '-'
	    smz->getsymbol (cursym);
	    if (cursym.get_value() == initialvalue) { //i.e. devswitch is 'initialvalue'
	      smz->getsymbol (cursym);
	      if (cursym.get_type() == Uint &&
		  (cursym.get_uint() == 0 || cursym.get_uint() == 1)) { // i.e. symbol is a uint but is also a bool
		return; // we have a valid SWITCH
	      } else {errcount++; throw boolex_i;} // expected a bl
	    } else {errcount++; throw swinitialvalueex_i;}// expected '-initialvalue'
      	  } else {errcount++; throw devdashex_i;} // expected '-'
	}

	if (cursym.get_value() == AND) { //i.e. devname is AND
	  smz->getsymbol (cursym);
	  if (cursym.get_value() == dash) { // i.e. symbol is '-'
	    smz->getsymbol (cursym);
	    if (cursym.get_value() == numinputs) { //i.e. devswitch is 'numinputs'
	      smz->getsymbol (cursym);
	      if (cursym.get_type() == Uint) { // i.e. symbol is a uint
		return; // we have a valid AND
	      } else {errcount++; throw uintex_i;} // expected a uint
	    } else {errcount++; throw swnuminputsex_i;} // expected 'numinputs'
	  } else {errcount++; throw devdashex_i;} // expected '-'

	}

	if (cursym.get_value() == NAND) { //i.e. devname is NAND
	  smz->getsymbol (cursym);
	  if (cursym.get_value() == dash) { // i.e. symbol is '-'
	    smz->getsymbol (cursym);
	    if (cursym.get_value() == numinputs) { //i.e. devswitch is 'numinputs'
	      smz->getsymbol (cursym);
	      if (cursym.get_type() == Uint) { // i.e. symbol is a uint
		return; // we have a valid NAND
	      } else {errcount++; throw uintex_i;} // expected a uint
	    } else {errcount++; throw swnuminputsex_i;} // expected 'numinputs'
	  } else {errcount++; throw devdashex_i;} // expected '-'
	}

	if (cursym.get_value() == OR) { //i.e. devname is OR
	  smz->getsymbol (cursym);
	  if (cursym.get_value() == dash) { // i.e. symbol is '-'
	    smz->getsymbol (cursym);
	    if (cursym.get_value() == numinputs) { //i.e. devswitch is '-numinputs'
	      smz->getsymbol (cursym);
	      if (cursym.get_type() == Uint) { // i.e. symbol is a uint
		return; // we have a valid OR
	      } else {errcount++; throw uintex_i;} // expected a uint
	    } else {errcount++; throw swnuminputsex_i;} // expected 'numinputs'
	  } else {errcount++; throw devdashex_i;} // expected '-'
	}

	if (cursym.get_value() == NOR) { //i.e. devname is NOR
	  smz->getsymbol (cursym);
	  if (cursym.get_value() == dash) { // i.e. symbol is '-'
	    smz->getsymbol (cursym);
	    if (cursym.get_value() == numinputs) { //i.e. devswitch is '-numinputs'
	      smz->getsymbol (cursym);
	      if (cursym.get_type() == Uint) { // i.e. symbol is a uint
		return; // we have a valid NOR
	      } else {errcount++; throw uintex_i;} // expected a uint
	    } else {errcount++; throw swnuminputsex_i;} // expected 'numinputs'
	  } else {errcount++; throw devdashex_i;} // expected '-'
	}

	if (cursym.get_value() == DTYPE) { //i.e. devname is DTYPE
	  return; // we have a valid DTYPE
	}

	if (cursym.get_value() == XOR) { //i.e. devname is XOR
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

class unameex : public exception
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected a uname";
  }
} unameex_i;

void parser::uname (void) // throws unameex if not given a valid uname
{
try
  {
//    cout<<"running uname"<<endl;
    if (cursym.get_type() != Uname) {errcount++; throw unameex_i;} // didn't get a uname
  }
 catch (exception& e)
   {
//     //cout << e.what() << endl;
//	smz->print_err(e.what());
     throw;
   }
}

class rulesymbolex : public exception {};

class rulesymequalsex : public rulesymbolex
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected an '='";
  }
} rulesymequalsex_i;

class rulesymrarrowex : public rulesymbolex
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected a '>'";
  }
} rulesymrarrowex_i
;
class rulesymsemicolonex : public rulesymbolex
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected a ';'";
  }
} rulesymsemicolonex_i;

class rulesymdotex : public rulesymbolex
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected a '.'";
  }
} rulesymdotex_i;

//class monruleex : public exception
//{
//	public:
//  virtual const char* what () const throw ()
//  {
//    return "Exception: invalid monrule in definition file";
//  }
//} monruleex_i;

void parser::monrule (void) // throws monruleex if finds invalid monrule
{
try
  {
    //cout<<"testing monrule"<<endl;
    uname();
	//TODO:check if this uname is in nametable (once James has finished that
	//do this for all three *rule functions
    smz->getsymbol (cursym);
    if (cursym.get_value() == dot) { // cursym is '.'
      smz->getsymbol (cursym);
      outname();
	  smz->getsymbol (cursym);
    } 
	if (cursym.get_value() == semicolon) { // cursym is ';'
      return;
    } else {errcount++; throw rulesymsemicolonex_i;} // didn't get a '.' or ';'
  }
 catch (exception& e) // Only catches an error in monrule syntax. Does not
                      // throw it further, so subsequent lines can contine
					  // to be parsed
   {
	smz->print_err(e.what());
     while (cursym.get_value() != semicolon && cursym.get_value() != endfile) { // iterates while cursym is not ';'
       smz->getsymbol (cursym);
	   //throw parseex_i;
     }
   }
}

//class connruleex : public exception
//{
//	public:
//  virtual const char* what () const throw ()
//  {
//    return "Exception: invalid connrule in definition file";
//  }
//} connruleex_i;

void parser::connrule (void) // throws connruleex if finds invalid connrule
{
try
  {
//    cout<<"testing conrule 1"<<endl;
    uname();
    smz->getsymbol (cursym);
    if (cursym.get_value() == dot) { // cursym is '.'
    smz->getsymbol (cursym);
    outname();
    } else if (cursym.get_value() == rarrow) { // cursym is '>'
    smz->getsymbol (cursym);
//    cout<<"testing conrule 2"<<endl;
    uname();
    smz->getsymbol (cursym);
    if (cursym.get_value() == dot) { // cursym is '.'
      smz->getsymbol (cursym);
      inname();
      smz->getsymbol (cursym);
      if (cursym.get_value() == semicolon) { // cursym is ';'
	return;
      } else {errcount++; throw rulesymsemicolonex_i;} // we expected a ';'
    } else {errcount++; throw rulesymdotex_i;} // we expected a '.'
    } else {errcount++; throw rulesymrarrowex_i;} // we expected a '>'
  }
 catch (exception& e)
   {
     //cout << e.what() << endl;
	smz->print_err(e.what());
     while (cursym.get_value() != semicolon && cursym.get_value() != endfile) { // iterates while cursym is not ';'
       smz->getsymbol (cursym);
     }
    //throw parseex_i;
   }
}

//class devruleex : public exception
//{
//	public:
//  virtual const char* what () const throw ()
//  {
//    return "Exception: invalid devrule in definition file";
//  }
//} devruleex_i;

void parser::devrule (void) // throws devruleex if finds invalid devrule
{
try
  {
//    cout<<"starting devrule"<<endl;
    device();
//    cout<<"device ran"<<endl;
    smz->getsymbol (cursym);
    if (cursym.get_value() == equals) { // we have an '='
//	cout<<"= found, starting uname"<<endl; 
	smz->getsymbol (cursym);     
	uname();
	smz->getsymbol (cursym);
      if (cursym.get_value() == semicolon) { // we have a ';'
	return;
      } else {errcount++; throw rulesymsemicolonex_i;} // we expected a ';'
    } else {errcount++; throw rulesymequalsex_i;} // we expected an '='
  }
 catch (exception& e)
   {
    // cout << e.what() << endl;
	smz->print_err(e.what());
     while (cursym.get_value() != semicolon && cursym.get_value() != endfile) { // iterates while cursym is not ';'
       smz->getsymbol (cursym);
     }
    //throw parseex_i;
   }
}

class sectionex : public exception
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: invalid Section in definition file";
  }
} sectionex_i;

class lbrakex : public sectionex
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected '{' at beginning of Section";
  }
} lbrakex_i;

class rbrakex : public sectionex
{
	public:
  virtual const char* what () const throw ()
  {
    return "Exception: expected '}' at beginning of Section";
  }
} rbrakex_i;

void parser::section (void) // throws sectionex if finds invalid section
{
try
  {
    smz->getsymbol (cursym);
     if (cursym.get_value() == endfile) return;
     if (cursym.get_value() == DEVICES) { // cursym is 'DEVICES'     
	smz->getsymbol (cursym);
      if (cursym.get_value() == lbrak) { // cursym is '{'	
	smz->getsymbol (cursym);
	while (cursym.get_value() != rbrak && cursym.get_value() != endfile) { // i.e. while inside braces
	  devrule();
	  smz->getsymbol (cursym);
	}
      } else {errcount++; throw lbrakex_i;} // we expected an '{'

    } else if (cursym.get_value() == CONNECTIONS) { // cursym is 'CONNECTIONS'
      smz->getsymbol (cursym);
      if (cursym.get_value() == lbrak) { // cursym is '{'
	smz->getsymbol (cursym);
	while (cursym.get_value() != rbrak && cursym.get_value() != endfile) { // i.e. while inside braces
	  connrule();
	  smz->getsymbol (cursym);
	}
      } else {errcount++; throw lbrakex_i;} // we expected an '{'

    } else if (cursym.get_value() == MONITOR) { // cursym is 'MONITOR'
      smz->getsymbol (cursym);
      if (cursym.get_value() == lbrak) { // cursym is '{'
	smz->getsymbol (cursym);
	while (cursym.get_value() != rbrak && cursym.get_value() != endfile) { // i.e. while inside braces
	  monrule();
	 // cout << "monrule ran" << endl;
	  smz->getsymbol (cursym);
	}
      } else {errcount++; throw lbrakex_i;} // we expected an '{'
    } else {errcount++; throw sectionex_i;} // didn't get a valid section keyword
  }
catch (sectionex& e)
   {
     //cout << e.what() << endl;
	smz->print_err(e.what());
	while (!(cursym.get_value() == rbrak)) { // iterates while cursym is not '}'
       smz->getsymbol (cursym);
     }
     //throw parseex_i;
   }
}

void parser::parsedeffile (void)
{
	try {
  do {
//cout << "start parsedeffile" << endl;
    /*   cout << "type_var: " << cursym.get_type() << endl;
    cout << "value_var: " << cursym.get_value() << endl;
    cout << "uint_var: " << cursym.get_uint() << endl;
    cout << "uname_var: " << cursym.get_uname() << endl; */
    section();
  } while (!(cursym.get_value() == endfile)); // i.e. while not eof
}
catch (exception& e) {
	cout << "parsedeffileexception: " << e.what() << endl;
	//throw;
}
}
