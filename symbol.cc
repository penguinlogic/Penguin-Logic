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
  uname_var = "\0";
}


/* Exception for get_<thing> failure  */
class get_symboldataexception : public exception {
  virtual const char* what () const throw ()
  {
    return "Exception: Type or Value requested from Symbol object has not been initialised";
  }
} get_symboldataexception_i;

void symbol::set_parameters (type in_type, value in_value, int in_uint, string in_uname){
type_var=in_type;
value_var=in_value;
uint_var=in_uint;
uname_var=in_uname;
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
 // try {
   // cout << "type_var: " << type_var << endl;
   // cout << "value_var: " << value_var << endl;
    //cout << "uint_var: " << uint_var << endl;
    //cout << "uname_var: " << uname_var << endl;
    //if (type_var != notype &&
	//type_var != Uint &&
	//type_var != Uname)
      return value_var;

    //throw get_symboldataexception_i; // the value held was not of type 'Value'
//  }
 // catch (exception& e) {
   // cout << e.what() << endl;
    //throw;
    //return novalue;
  //}
}

/* returns uint if there is one */
int symbol::get_uint () {
 // try {
   // if (type_var == Uint){
	//cout<<"getting uint"<<endl;
      return uint_var;
//	}
    //throw get_symboldataexception_i; // the value held was not of type 'Uint'
 //}
  //catch (exception& e) {
    //cout << e.what() << endl;
   // throw;
   // return -1;
//  } 
}

/* returns uname if there is one */
string symbol::get_uname () {
//  try {
  //  if ( type_var == Uname)
      return uname_var;
//else return "\0";

    //throw get_symboldataexception_i; // the value held was not of type 'Uname'
 // }
 // catch (exception& e) {
   // cout << e.what() << endl;
   // throw;
   // return "\0";
//  }
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

/* performs inequality operation */
bool symbol::operator!= (symbol rhs) {
  if (type_var == rhs.get_type() &&
      value_var == rhs.get_value() &&
      uint_var == rhs.get_uint() &&
      uname_var == rhs.get_uname() )
    return false;

  return true;
}

/* performs assignment operation */
symbol symbol::operator= (symbol rhs) {
  type_var = rhs.get_type();
  value_var = rhs.get_value();
  uint_var = rhs.get_uint();
  uname_var = rhs.get_uname();
  return *this;
}
