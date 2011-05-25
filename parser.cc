#include <iostream>
#include "parser.h"

using namespace std;

/* The parser for the circuit definition files */


bool parser::readin (void)
{
  /* over to you */
	// return false to allow program to compile
	return false;
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
    if (typeid(cursym) != typeid(bool i)) throw boolruleex;
  }
catch(exception& e)
  {
    cout << e.what() << endl;
  }
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
    if (typeid(cursym) != typeid(unsigned int i)) throw uintex;
  }
catch(exception& e)
  {
    cout << e.what() << endl;
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
    if (cursym == isym)
      {
	getsymbol (cursym, curid, curnum);
	uint();
      }
    else if (cursym == datasym);
    else if (cursym == setsym);
    else if (cursym == clearsym);
    else if (cursym == clksym);
    else throw innameex;
  }
 catch (exception& e)
   {
     cout << e.what() << endl;
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
    if (cursym != qsym && cursym != qbarsym) throw outnameex;
  }
 catch (exception& e)
   {
     cout << e.what() << endl;
   }
}

class deviceex : public exception
{
  virtual const char* what () const throw ()
  {
    return "Exception: invalid device in definition file";
  }
}

void parser::device (void) // throws deviceex if not given a valid device
{
try
  {
    if (cursym == clocksym)
      {
	getsymbol (cursym, curid, curnum);
	if (cursym == periodsym)
	  {
	    getsymbol (cursym, curid, curnum);
	    uint();
	  }
	else throw deviceex;
      }

    else if (cursym == switchsym)
      {
	getsymbol (cursym, curid, curnum);
	if (cursym == initialvaluesym)
	  {
	    getsymbol (cursym, curid, curnum);
	    boolrule();
	  }
	else throw deviceex;
      }

    else if (cursym == andsym)
      {
	getsymbol (cursym, curid, curnum);
	if (cursym == numinputssym)
	  {
	    getsymbol (cursym, curid, curnum);
	    uint();
	  }
	else throw deviceex;
      }

    else if (cursym == nandsym)
      {
	getsymbol (cursym, curid, curnum);
	if (cursym == numinputssym)
	  {
	    getsymbol (cursym, curid, curnum);
	    uint();
	  }
	else throw deviceex;
      }

    else if (cursym == orsym)
      {
	getsymbol (cursym, curid, curnum);
	if (cursym == numinputssym)
	  {
	    getsymbol (cursym, curid, curnum);
	    uint();
	  }
	else throw deviceex;
      }

    else if (cursym == norsym)
      {
	getsymbol (cursym, curid, curnum);
	if (cursym == numinputssym)
	  {
	    getsymbol (cursym, curid, curnum);
	    uint();
	  }
	else throw deviceex;
      }

    else if (cursym == dtypesym);

    else if (cursym == xorsym);

    else throw deviceex;
  }
 catch (exception& e)
   {
     cout << e.what() << endl;
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

  }
 catch (exception& e)
   {
     cout << e.what() << endl;
   }
}
}
