#include <iostream>
#include "parser.h"

using namespace std;

/* The parser for the circuit definition files */


bool parser::readin (void)
{
  try {
    parsedeffile();
    return true:
  }
  catch {
    cout << "Failed to correctly parse definition file." << endl;
    return false;
  }
}

parser::parser (network* network_mod, devices* devices_mod,
		monitor* monitor_mod, scanner* scanner_mod)
{
  netz = network_mod;  /* make internal copies of these class pointers */
  dmz = devices_mod;   /* so we can call functions from these classes  */
  mmz = monitor_mod;   /* eg. to call makeconnection from the network  */
  smz = scanner_mod;   /* class you say:                               */
                       /* netz->makeconnection (i1, i2, o1, o2, ok);   */

  /* any other initialisation you want to do? */

}

class uintex : public exception
{
  virtual const char* what () const throw ()
  {
    return "Exception: invalid uint in definition file";
  }
}

void parser::uint (void) // throws uintex if not given a valid uint
{
try
  {
    if (cursym.get_type() != 0) throw uintex; // didn't get uint
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
}

void parser::boolrule (void) // throws boolruleex if not given a valid bool
{
try
  {
    if (cursym.get_type() != 1) throw boolruleex; // didn't get a bl
  }
catch(exception& e)
  {
    cout << e.what() << endl;
    throw;
  }
}

class innameex : public exception
{
  virtual const char* what () const throw ()
  {
    return "Exception: invalid inname in definition file";
  }
}

void parser::inname (void) // throws innameex if not given a valid inname
{
try
  {
    if (cursym.get_type() != 6) throw innameex; // didn't get an inname
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
}

void parser::outname (void) // throws outnameex if not given a valid outname
{
try
  {
    if (cursym.get_type() != 5) throw outnameex; // didn't get an outname
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
}

void parser::device (void) // throws deviceex if finds invalid device
{
try
  {
    if (cursym.get_type() == 3) { //i.e. if symbol is a 'devname'
	/* Now we need to test if the device is valid.
        We test each combination of devname, devswitch, and if
        necessary the next symbol to make sure of a valid device
	*/
	if (cursym.get_value() == 0) { //i.e. devname is CLOCK
	getsymbol (cursym, curid, curnum);
	if (cursym.get_type() == 4 &&
            cursym.get_value() == 0) { //i.e. devswitch is '-period'
	getsymbol (cursym, curid, curnum);
	if (cursym.get_type() == 0) { // i.e. symbol is a uint
	  return; // we have a valid CLOCK
	} else throw deviceex; // expected a uint
	} else throw deviceex; // expected '-period'
	}

	if (cursym.get_value() == 1) { //i.e. devname is SWITCH
	getsymbol (cursym, curid, curnum);
	if (cursym.get_type() == 4 &&
            cursym.get_value() == 1) { //i.e. devswitch is '-initialvalue'
	getsymbol (cursym, curid, curnum);
	if (cursym.get_type() == 1) { // i.e. symbol is a bl
	  return; // we have a valid SWITCH
	} else throw deviceex; // expected a bl
	} else throw deviceex; // expected '-initialvalue'
	}

	if (cursym.get_value() == 2) { //i.e. devname is AND
	getsymbol (cursym, curid, curnum);
	if (cursym.get_type() == 4 &&
            cursym.get_value() == 2) { //i.e. devswitch is '-numinputs'
	getsymbol (cursym, curid, curnum);
	if (cursym.get_type() == 0) { // i.e. symbol is a uint
	  return; // we have a valid AND
	} else throw deviceex; // expected a uint
	} else throw deviceex; // expected '-numinputs'
	}

	if (cursym.get_value() == 3) { //i.e. devname is NAND
	getsymbol (cursym, curid, curnum);
	if (cursym.get_type() == 4 &&
            cursym.get_value() == 2) { //i.e. devswitch is '-numinputs'
	getsymbol (cursym, curid, curnum);
	if (cursym.get_type() == 0) { // i.e. symbol is a uint
	  return; // we have a valid NAND
	} else throw deviceex; // expected a uint
	} else throw deviceex; // expected '-numinputs'
	}

	if (cursym.get_value() == 4) { //i.e. devname is OR
	getsymbol (cursym, curid, curnum);
	if (cursym.get_type() == 4 &&
            cursym.get_value() == 2) { //i.e. devswitch is '-numinputs'
	getsymbol (cursym, curid, curnum);
	if (cursym.get_type() == 0) { // i.e. symbol is a uint
	  return; // we have a valid OR
	} else throw deviceex; // expected a uint
	} else throw deviceex; // expected '-numinputs'
	}

	if (cursym.get_value() == 5) { //i.e. devname is NOR
	getsymbol (cursym, curid, curnum);
	if (cursym.get_type() == 4 &&
            cursym.get_value() == 2) { //i.e. devswitch is '-numinputs'
	getsymbol (cursym, curid, curnum);
	if (cursym.get_type() == 0) { // i.e. symbol is a uint
	  return; // we have a valid NOR
	} else throw deviceex; // expected a uint
	} else throw deviceex; // expected '-numinputs'
	}

	if (cursym.get_value() == 6) { //i.e. devname is DTYPE
	  return; // we have a valid DTYPE
	}

	if (cursym.get_value() == 7) { //i.e. devname is XOR
	  return; // we have a valid XOR
	}

	throw deviceex; // didn't get a valid devname
    } else throw deviceex; // didn't get a devname to start with
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
}

void parser::uname (void) // throws unameex if not given a valid uname
{
try
  {
    if (cursym.get_type() != 8) throw unameex; // didn't get a uname
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
}

void parser::monrule (void) // throws monruleex if finds invalid monrule
{
try
  {
    uname();
    getsymbol (cursym, curid, curnum);
    if (cursym.get_type() == 7 && cursym.get_value() == 5) { // cursym is '.'
      getsymbol (cursym, curid, curnum);
      outname();
    } else if (cursym.get_type() == 7 && cursym.get_value() == 4) { // cursym is ';'
      return;
    } else throw monruleex; // didn't get a '.' or ';'
  }
 catch (exception& e)
   {
     cout << e.what() << endl;
     while (!(cursym.get_type() == 7 && cursym.get_value() == 4)) { // iterates while cursym is not ';'
       getsymbol (cursym, curid, curnum);
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
}

void parser::connrule (void) // throws connruleex if finds invalid connrule
{
try
  {
    uname();
    getsymbol (cursym, curid, curnum);
    if (cursym.get_type() == 7 && cursym.get_value() == 5) { // cursym is '.'
    getsymbol (cursym, curid, curnum);
    outname();
    } else if (cursym.get_type() == 7 && cursym.get_value() == 3) { // cursym is '>'
    getsymbol (cursym, curid, curnum);
    uname();
    getsymbol (cursym, curid, curnum);
    if (cursym.get_type() == 7 && cursym.get_value() == 5) { // cursym is '.'
      getsymbol (cursym, curid, curnum);
      inname();
      getsymbol (cursym, curid, curnum);
      if (cursym.get_type() == 7 && cursym.get_value() == 4) { // cursym is ';'
	return;
      } else throw connruleex; // we expected a ';'
    } else throw connruleex; // we expected a '.'
    } else throw connruleex; // we expected a '>'
  }
 catch (exception& e)
   {
     cout << e.what() << endl;
     while (!(cursym.get_type() == 7 && cursym.get_value() == 4)) { // iterates while cursym is not ';'
       getsymbol (cursym, curid, curnum);
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
}

void parser::devrule (void) // throws devruleex if finds invalid devrule
{
try
  {
    device();
    getsymbol (cursym, curid, curnum);
    if (cursym.get_type() == 7 && cursym.get_value() == 2) { // we have an '='
      uname();
      if (cursym.get_type() == 7 && cursym.get_value() == 4) { // we have a ';'
	return;
      } else throw devruleex; // we expected a ';'
    } else throw devruleex; // we expected an '='
  }
 catch (exception& e)
   {
     cout << e.what() << endl;
     while (!(cursym.get_type() == 7 && cursym.get_value() == 4)) { // iterates while cursym is not ';'
       getsymbol (cursym, curid, curnum);
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
}

void parser::section (void) // throws sectionex if finds invalid section
{
try
  {
    if (cursym.get_type() == 2 && cursym.get_value() == 0) { // cursym is 'DEVICES'
      getsymbol (cursym, curid, curnum);
      if (cursym.get_type() == 7 && cursym.get_value() == 0) { // cursym is '{'
	getsymbol (cursym, curid, curnum);
	while (!(cursym.get_type() == 7 && cursym.get_value() == 1)) { // i.e. while inside braces
	  devrule;
	  getsymbol (cursym, curid, curnum);
	}
      } else throw sectionex; // we expected an '{'

    } else if (cursym.get_type() == 2 && cursym.get_value() == 1) { // cursym is 'CONNECTIONS'
      getsymbol (cursym, curid, curnum);
      if (cursym.get_type() == 7 && cursym.get_value() == 0) { // cursym is '{'
	getsymbol (cursym, curid, curnum);
	while (!(cursym.get_type() == 7 && cursym.get_value() == 1)) { // i.e. while inside braces
	  connrule;
	  getsymbol (cursym, curid, curnum);
	}
      } else throw sectionex; // we expected an '{'

    } else if (cursym.get_type() == 2 && cursym.get_value() == 2) { // cursym is 'MONITOR'
      getsymbol (cursym, curid, curnum);
      if (cursym.get_type() == 7 && cursym.get_value() == 0) { // cursym is '{'
	getsymbol (cursym, curid, curnum);
	while (!(cursym.get_type() == 7 && cursym.get_value() == 1)) { // i.e. while inside braces
	  monnrule;
	  getsymbol (cursym, curid, curnum);
	}
      } else throw sectionex; // we expected an '{'
    } else throw sectionex; // didn't get a valid section keyword
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
    getsymbol (cursym, curid, curnum);
    section();
  } while (!(cursym.get_type() == 7 && cursym.get_value() == 6)) // i.e. while not eof
}
