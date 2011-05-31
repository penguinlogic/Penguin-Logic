#include "symbol.h"

using namespace std;

/* Exception for constuctor failure */
class symbolconstructorexception : public exception {
  virtual const char* what () const throw ()
  {
    return "Exception: Assignment failed in symbol constructor";
  }
} symbolconstructorexception_i;

symbol::symbol () {
  type_var = notype;
  value_var = novalue;
  uint_var = -1;
  uname_id_var = 0;
}


/* Exception for get_<thing> failure  */
class get_symboldataexception : public exception {
  virtual const char* what () const throw ()
  {
    return "Exception: Type or Value requested from Symbol object has not been initialised";
  }
} get_symboldataexception_i;

void symbol::set_parameters (type in_type, value in_value, int in_uint, name in_uname_id){
type_var=in_type;
value_var=in_value;
uint_var=in_uint;
uname_id_var=in_uname_id;
}
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
      return value_var;
}

/* returns uint if there is one */
int symbol::get_uint () {
      return uint_var;
}

/* returns uname if there is one */
name symbol::get_uname_id () 
{
      return uname_id_var;
}

/* performs equality operation */
bool symbol::operator== (symbol rhs) {
  if (type_var == rhs.get_type() &&
      value_var == rhs.get_value() &&
      uint_var == rhs.get_uint() &&
      uname_id_var == rhs.get_uname_id() )
    return true;

  return false;
}

/* performs inequality operation */
bool symbol::operator!= (symbol rhs) {
  if (type_var == rhs.get_type() &&
      value_var == rhs.get_value() &&
      uint_var == rhs.get_uint() &&
      uname_id_var == rhs.get_uname_id() )
    return false;

  return true;
}

/* performs assignment operation */
symbol symbol::operator= (symbol rhs) {
  type_var = rhs.get_type();
  value_var = rhs.get_value();
  uint_var = rhs.get_uint();
  uname_id_var = rhs.get_uname_id();
  return *this;
}
