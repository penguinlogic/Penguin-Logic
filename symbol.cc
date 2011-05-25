#include "symbol.h"

using namespace std;

/* Exception for constuctor failure */
class symbolconstructorexception : public exception {
  virtual const char* what () const throw ()
  {
    return "Exception: Assigment failed in symbol constructor";
  }
}

/*  Constructor assigns type_var (an enum describing the
type of symbol contained by the object) and one variable
(e.g. uint_var, bl_var, section_var, etc.) containing the
value of that symbol.
 */

symbol::symbol (int symtype, int num) {
  try {
    if (symtype == 0) { // symtype is 'uint'
      type_var = symtype;
      if (num > 0 && num < 17) {
	uint_var = num;
      }

    } else if (symtype == 1) { // symtype is 'bl'
      type_var = symtype;
      if (num == 0 || num == 1) {
	bl_var = num;
      }

    } else if (symtype == 2) { // symtype is 'section'
      type_var = symtype;
      if (num > -1 && num < 4) {
	section_var = num;
      }

    } else if (symtype == 3) { // symtype is 'devname'
      type_var = symtype;
      if (num > -1 && num < 8) {
	devtype_var = num;
      }

    } else if (symtype == 4) { // symtype is 'devswitch'
      type_var = symtype;
      if (num > -1 && num < 3) {
	devswitch_var = num;
      }

    } else if (symtype == 5) { // symtype is 'outname'
      type_var = symtype;
      if (num > -1 && num < 2) {
	outname_var = num;
      }

    } else if (symtype == 6) { // symtype is 'inname'
      type_var = symtype;
      if (num > -1 && num < 21) {
	inname_var = num;
      }

    } else if (symtype == 7) { // symtype is 'charsym'
      type_var = symtype;
      if (num > -1 && num < 5) {
	inname_var = num;
      }
    } else throw symbolconstructorexception;
  }

  catch (exception& e) {
    cout << e.what() << endl;
  }
}

/* Special constructor needed for unames because we
cannot enum uname - it can be any string. Therefore we
pass the string in and store it directly
*/

symbol::symbol (int symtype, char* name_in) {
  try {
    if (symtype == 8) {
      type_var = symtype;
      uname_var = name_in;
    } else {
      throw symbolconstructorexception;
    }
  }
  catch (exception& e) {
    cout << e.what() << endl;
  }
}

/* Exception for get_<thing> failure */
class get_symboldataexception : public exception {
  virtual const char* what () const throw ()
  {
    return "Exception: Type or Value requested from Symbol object has not been initialised";
  }
}

/* Returns the type of symbol held in the object*/
int symbol::get_type () {
  try {
    if ( type_var != -1) return type_var;
    else throw get_symboldataexception;
  }
  catch (exception& e) {
    cout << e.what() << endl;
  }
}

/* Returns the value of symbol held in the object (for
all except unames
*/
int symbol::get_value (int symtype) { // symtype is 'uint'
  try {
    if (symtype == 0) {
      if ( uint_var != -1) return uint_var;
      else throw get_symboldataexception;

    } else if (symtype == 1) { // symtype is 'bl'
      if ( bl_var != -1) return bl_var;
      else throw get_symboldataexception;

    } else if (symtype == 2) { // symtype is 'section'
      if ( section_var != -1) return section_var;
      else throw get_symboldataexception;

    } else if (symtype == 3) { // symtype is 'devname'
      if ( devname_var != -1) return devname_var;
      else throw get_symboldataexception;

    } else if (symtype == 4) { // symtype is 'devswitch'
      if ( devswitch_var != -1) return devswitch_var;
      else throw get_symboldataexception;

    } else if (symtype == 5) { // symtype is 'outname'
      if ( outname_var != -1) return outname_var;
      else throw get_symboldataexception;

    } else if (symtype == 6) { // symtype is 'inname'
      if ( inname_var != -1) return inname_var;
      else throw get_symboldataexception;

    } else if (symtype == 7) { // symtype is 'charsym'
      if ( charsym_var != -1) return charsym_var;
      else throw get_symboldataexception;

    } else throw get_symboldataexception;
  }

  catch (exception& e) {
    cout << e.what() << endl;
  }
}

/* returns uname if there is one */
char* symbol::get_uname () {
  try {
    if ( uname_var != '\0') return uname_var;
    else throw get_symboldataexception;
  }
  catch (exception& e) {
    cout << e.what() << endl;
  }
}

/* performs equality operation */
bool symbol::operator== (symbol rhs) {
  symbol lhs;
  if (type_var == rhs.type_var &&
      uint_var == rhs.uint_var &&
      bl_var == rhs.bl_var &&
      section_var == rhs.section_var &&
      devname_var == rhs.devname_var &&
      devswitch_var == rhs.devswitch_var &&
      outname_var == rhs.outname_var &&
      inname_var == rhs.inname_var &&
      charsym_var == rhs.charsym_var &&
      uname_var == rhs.uname_var) return true;
  else return false;
}
