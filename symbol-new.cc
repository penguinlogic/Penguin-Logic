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
  id_var = blankname;
  uint_var = -1;
}


/* Exception for get_<thing> failure  */
class get_symboldataexception : public exception {
  virtual const char* what () const throw ()
  {
    return "Exception: Type or Value requested from Symbol object has not been initialised";
  }
} get_symboldataexception_i;

void symbol::set_parameters (type in_type, name in_id, int in_uint,){
type_var=in_type;
id_var=in_id;
uint_var=in_uint;
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
value symbol::get_id () {
      return id_var;
}

/* returns uint if there is one */
int symbol::get_uint () {
      return uint_var;
}


/* performs equality operation */
bool symbol::operator== (symbol rhs) {
  if (type_var == rhs.get_type() &&
      id_var == rhs.get_id() &&
      uint_var == rhs.get_uint())
    return true;

  return false;
}

/* performs inequality operation */
bool symbol::operator!= (symbol rhs) {
  if (type_var == rhs.get_type() &&
      id_var == rhs.get_id() &&
      uint_var == rhs.get_uint() &&
      return false;

  return true;
}

/* performs assignment operation */
symbol symbol::operator= (symbol rhs) {
  type_var = rhs.get_type();
  id_var = rhs.get_id();
  uint_var = rhs.get_uint();
  return *this;
}
