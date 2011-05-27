#include "symbol.h"

using namespace std;

/* Exception for constuctor failure */
class symbolconstructorexception : public exception {
  virtual const char* what () const throw ()
  {
    return "Exception: Assigment failed in symbol constructor";
  }
} symbolconstructorexception_i;

/* Exception for uname conditions failure */
class symbolconstructorunameexception : public symbolconstructorexception {
  virtual const char* what () const throw ()
  {
    return "Exception: Invalid 'uname' found by symbol constructor";
  }
} symbolconstructorunameexception_i;

/*  Constructor assigns type_var (an enum describing the
type of symbol contained by the object) and one variable
(e.g. uint_var, bl_var, section_var, etc.) containing the
value of that symbol.
 */

symbol::symbol (type in_type, value in_value, int in_uint, string in_uname) {
  type_var = notype;
  value_var = novalue;
  uint_var = -1;
  uname_var = "\0";

  try {
    type_var = in_type;

    if (type_var != notype &&
	type_var != Uint &&
	type_var != Uname)
      value_var = in_value;

    else if (type_var == Uint)
      uint_var = in_uint;

    else if (type_var == Uname)
      uname_var = in_uname;

    else throw symbolconstructorexception_i;
  }
  catch (exception& e) {
    cout << e.what() << endl;
    throw;
  }
}


/* Exception for get_<thing> failure */
class get_symboldataexception : public exception {
  virtual const char* what () const throw ()
  {
    return "Exception: Type or Value requested from Symbol object has not been initialised";
  }
} get_symboldataexception_i;

/* Returns the type of symbol held in the object*/
type symbol::get_type () {
  try {
    if ( type_var != notype) return type_var;
    else throw get_symboldataexception_i;
  }
  catch (exception& e) {
    cout << e.what() << endl;
    throw;
    return notype;
  }
}

/* Returns the value of symbol held in the object (for
all except unames
*/
value symbol::get_value () {
  try {
    if (type_var != notype &&
	type_var != Uint &&
	type_var != Uname)
      return value_var;

    throw get_symboldataexception_i; // the value held was not of type 'Value'
  }
  catch (exception& e) {
    cout << e.what() << endl;
    throw;
    return novalue;
  }
}

/* returns uint if there is one */
int symbol::get_uint () {
  try {
    if (type_var == Uint)
      return uint_var;

    throw get_symboldataexception_i; // the value held was not of type 'Uint'
  }
  catch (exception& e) {
    cout << e.what() << endl;
    throw;
    return -1;
  }
}

/* returns uname if there is one */
string symbol::get_uname () {
  try {
    if ( type_var == Uname)
      return uname_var;

    throw get_symboldataexception_i; // the value held was not of type 'Uname'
  }
  catch (exception& e) {
    cout << e.what() << endl;
    throw;
    return "\0";
  }
}

/* performs equality operation */
bool symbol::operator== (symbol rhs) {
  if (type_var == rhs.get_type() &&
      value_var == rhs.get_value() &&
      uint_var == rhs.get_uint() &&
      uname_var == rhs.get_uname() )
    return true;

  return false;
}

/* performs assignment operation */
symbol symbol::operator= (symbol rhs) {
  type_var = rhs.get_type();
  value_var = rhs.get_value();
  uint_var = rhs.get_uint();
  uname_var = rhs.get_uname();
  return *this;
}
