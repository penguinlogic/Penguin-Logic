#include <iostream>
#include "parser.h"

using namespace std;

/* The parser for the circuit definition files */


bool parser::readin (void)
{
  try {
    parsedeffile();
    cout << "It worked!" << endl;
    return true;
  }
  catch (exception& e) {
    cout << "Failed to correctly parse definition file." << endl;
    return false;
  }
}

parser::parser (/*network* network_mod, devices* devices_mod,
		monitor* monitor_mod,*/ scanner* scanner_mod)
{
cout << "parser constructor" << endl;
  //netz = network_mod;  /* make internal copies of these class pointers */
  //dmz = devices_mod;   /* so we can call functions from these classes  */
  //mmz = monitor_mod;   /* eg. to call makeconnection from the network  */
  smz = scanner_mod;   /* class you say:                               */
                       /* netz->makeconnection (i1, i2, o1, o2, ok);   */

 // cursym = *new symbol();
	symbol cursym;

}

class uintex : public exception
{
  virtual const char* what () const throw ()
  {
    return "Exception: invalid uint in definition file";
  }
} uintex_i;

void parser::uint (void) // throws uintex if not given a valid uint
{
try
  {
    if (cursym.get_type() != Uint) throw uintex_i; // didn't get uint
  }
catch(exception& e)
  {
    cout << e.what() << endl;
    throw;
  }
}

class boolruleex : public exception
{
  virtual const char* what () const throw ()
  {
    return "Exception: invalid bool in definition file";
  }
} boolruleex_i;


class innameex : public exception
{
  virtual const char* what () const throw ()
  {
    return "Exception: invalid inname in definition file";
  }
} innameex_i;

void parser::inname (void) // throws innameex if not given a valid inname
{
try
  {
    if (cursym.get_type() != Inname) throw innameex_i; // didn't get an inname
  }
 catch (exception& e)
   {
     cout << e.what() << endl;
     throw;
   }
}

class outnameex : public exception
{
  virtual const char* what () const throw ()
  {
    return "Exception: invalid outname in definition file";
  }
} outnameex_i;

void parser::outname (void) // throws outnameex if not given a valid outname
{
try
  {
    if (cursym.get_type() != Outname) throw outnameex_i; // didn't get an outname
  }
 catch (exception& e)
   {
     cout << e.what() << endl;
     throw;
   }
}

class deviceex : public exception
{
  virtual const char* what () const throw ()
  {
    return "Exception: invalid device in definition file";
  }
} deviceex_i;

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
	      } else throw deviceex_i; // expected a uint
	    } else throw deviceex_i; // expected 'period'
	  } else throw deviceex_i; // expected '-'
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
	      } else throw deviceex_i; // expected a bl
	    } else throw deviceex_i; // expected '-initialvalue'
      	  } else throw deviceex_i; // expected '-'
	}

	if (cursym.get_value() == AND) { //i.e. devname is AND
	  smz->getsymbol (cursym);
	  if (cursym.get_value() == dash) { // i.e. symbol is '-'
	    smz->getsymbol (cursym);
	    if (cursym.get_value() == numinputs) { //i.e. devswitch is 'numinputs'
	      smz->getsymbol (cursym);
	      if (cursym.get_type() == Uint) { // i.e. symbol is a uint
		return; // we have a valid AND
	      } else throw deviceex_i; // expected a uint
	    } else throw deviceex_i; // expected '-numinputs'
      	  } else throw deviceex_i; // expected '-'
	}

	if (cursym.get_value() == NAND) { //i.e. devname is NAND
	  smz->getsymbol (cursym);
	  if (cursym.get_value() == dash) { // i.e. symbol is '-'
	    smz->getsymbol (cursym);
	    if (cursym.get_value() == numinputs) { //i.e. devswitch is 'numinputs'
	      smz->getsymbol (cursym);
	      if (cursym.get_type() == Uint) { // i.e. symbol is a uint
		return; // we have a valid NAND
	      } else throw deviceex_i; // expected a uint
	    } else throw deviceex_i; // expected '-numinputs'
      	  } else throw deviceex_i; // expected '-'
	}

	if (cursym.get_value() == OR) { //i.e. devname is OR
	  smz->getsymbol (cursym);
	  if (cursym.get_value() == dash) { // i.e. symbol is '-'
	    smz->getsymbol (cursym);
	    if (cursym.get_value() == numinputs) { //i.e. devswitch is '-numinputs'
	      smz->getsymbol (cursym);
	      if (cursym.get_type() == Uint) { // i.e. symbol is a uint
		return; // we have a valid OR
	      } else throw deviceex_i; // expected a uint
	    } else throw deviceex_i; // expected '-numinputs'
      	  } else throw deviceex_i; // expected '-'
	}

	if (cursym.get_value() == NOR) { //i.e. devname is NOR
	  smz->getsymbol (cursym);
	  if (cursym.get_value() == dash) { // i.e. symbol is '-'
	    smz->getsymbol (cursym);
	    if (cursym.get_value() == numinputs) { //i.e. devswitch is '-numinputs'
	      smz->getsymbol (cursym);
	      if (cursym.get_type() == Uint) { // i.e. symbol is a uint
		return; // we have a valid NOR
	      } else throw deviceex_i; // expected a uint
	    } else throw deviceex_i; // expected '-numinputs'
      	  } else throw deviceex_i; // expected '-'
	}

	if (cursym.get_value() == DTYPE) { //i.e. devname is DTYPE
	  return; // we have a valid DTYPE
	}

	if (cursym.get_value() == XOR) { //i.e. devname is XOR
	  return; // we have a valid XOR
	}

	throw deviceex_i; // didn't get a valid devname
    } else throw deviceex_i; // didn't get a devname to start with
  }
 catch (exception& e)
   {
     cout << e.what() << endl;
     throw;
   }
}

class unameex : public exception
{
  virtual const char* what () const throw ()
  {
    return "Exception: invalid uname in definition file";
  }
} unameex_i;

void parser::uname (void) // throws unameex if not given a valid uname
{
try
  {
    cout<<"running uname"<<endl;
    if (cursym.get_type() != Uname) throw unameex_i; // didn't get a uname
  }
 catch (exception& e)
   {
     cout << e.what() << endl;
     throw;
   }
}

class monruleex : public exception
{
  virtual const char* what () const throw ()
  {
    return "Exception: invalid monrule in definition file";
  }
} monruleex_i;

void parser::monrule (void) // throws monruleex if finds invalid monrule
{
try
  {
    cout<<"testing monrule"<<endl;
    uname();
    smz->getsymbol (cursym);
    if (cursym.get_value() == dot) { // cursym is '.'
      smz->getsymbol (cursym);
      outname();
    } else if (cursym.get_value() == semicolon) { // cursym is ';'
      return;
    } else throw monruleex_i; // didn't get a '.' or ';'
  }
 catch (exception& e)
   {
     cout << e.what() << endl;
     while (!(cursym.get_value() == semicolon)) { // iterates while cursym is not ';'
       smz->getsymbol (cursym);
     }
     throw;
   }
}

class connruleex : public exception
{
  virtual const char* what () const throw ()
  {
    return "Exception: invalid connrule in definition file";
  }
} connruleex_i;

void parser::connrule (void) // throws connruleex if finds invalid connrule
{
try
  {
    cout<<"testing conrule 1"<<endl;
    uname();
    smz->getsymbol (cursym);
    if (cursym.get_value() == dot) { // cursym is '.'
    smz->getsymbol (cursym);
    outname();
    } else if (cursym.get_value() == rarrow) { // cursym is '>'
    smz->getsymbol (cursym);
    cout<<"testing conrule 2"<<endl;
    uname();
    smz->getsymbol (cursym);
    if (cursym.get_value() == dot) { // cursym is '.'
      smz->getsymbol (cursym);
      inname();
      smz->getsymbol (cursym);
      if (cursym.get_value() == semicolon) { // cursym is ';'
	return;
      } else throw connruleex_i; // we expected a ';'
    } else throw connruleex_i; // we expected a '.'
    } else throw connruleex_i; // we expected a '>'
  }
 catch (exception& e)
   {
     cout << e.what() << endl;
     while (!(cursym.get_value() == semicolon)) { // iterates while cursym is not ';'
       smz->getsymbol (cursym);
     }
     throw;
   }
}

class devruleex : public exception
{
  virtual const char* what () const throw ()
  {
    return "Exception: invalid devrule in definition file";
  }
} devruleex_i;

void parser::devrule (void) // throws devruleex if finds invalid devrule
{
try
  {
    cout<<"starting devrule"<<endl;
    device();
    cout<<"device ran"<<endl;
    smz->getsymbol (cursym);
    if (cursym.get_value() == equals) { // we have an '='
	cout<<"= found, starting uname"<<endl; 
	smz->getsymbol (cursym);     
	uname();
	smz->getsymbol (cursym);
      if (cursym.get_value() == semicolon) { // we have a ';'
	return;
      } else throw devruleex_i; // we expected a ';'
    } else throw devruleex_i; // we expected an '='
  }
 catch (exception& e)
   {
     cout << e.what() << endl;
     while (!(cursym.get_value() == semicolon)) { // iterates while cursym is not ';'
       smz->getsymbol (cursym);
     }
     throw;
   }
}

class sectionex : public exception
{
  virtual const char* what () const throw ()
  {
    return "Exception: invalid section in definition file";
  }
} sectionex_i;

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
	while (cursym.get_value() != rbrak) { // i.e. while inside braces
	  devrule();
	  smz->getsymbol (cursym);
	}
      } else throw sectionex_i; // we expected an '{'

    } else if (cursym.get_value() == CONNECTIONS) { // cursym is 'CONNECTIONS'
      smz->getsymbol (cursym);
      if (cursym.get_value() == lbrak) { // cursym is '{'
	smz->getsymbol (cursym);
	while (!(cursym.get_value() == rbrak)) { // i.e. while inside braces
	  connrule();
	  smz->getsymbol (cursym);
	}
      } else throw sectionex_i; // we expected an '{'

    } else if (cursym.get_value() == MONITOR) { // cursym is 'MONITOR'
      smz->getsymbol (cursym);
      if (cursym.get_value() == lbrak) { // cursym is '{'
	smz->getsymbol (cursym);
	while (!(cursym.get_value() == rbrak)) { // i.e. while inside braces
	  monrule();
	  smz->getsymbol (cursym);
	}
      } else throw sectionex_i; // we expected an '{'
    } else throw sectionex_i; // didn't get a valid section keyword
  }
catch (exception& e)
   {
     cout << e.what() << endl;
     throw;
   }
}

void parser::parsedeffile (void)
{
  do {
cout << "start parsedeffile" << endl;
    /*   cout << "type_var: " << cursym.get_type() << endl;
    cout << "value_var: " << cursym.get_value() << endl;
    cout << "uint_var: " << cursym.get_uint() << endl;
    cout << "uname_var: " << cursym.get_uname() << endl; */
    section();
  } while (!(cursym.get_value() == endfile)); // i.e. while not eof
}
