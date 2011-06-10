#include "symbol.h"

using namespace std;

/* Exception for constuctor failure */
class symbolconstructorexception:public exception {
    virtual const char *what() const throw() {
	return "Exception: Assignment failed in symbol constructor";
    }
}
symbolconstructorexception_i;

symbol::symbol()
{
    type_var = notype;
    syntaxvalue_var = novalue;
    uint_var = -1;
    name_id_var = 0;
}

/* Exception for get_<thing> failure  */
class get_symboldataexception:public exception {
    virtual const char *what() const throw() {
	return
	    "Exception: Type or syntaxvalue requested from Symbol object has not been initialised";
    }
}
get_symboldataexception_i;

void
 symbol::set_parameters(type in_type, syntaxvalue in_syntaxvalue, int in_uint,
			name in_name_id)
{
    type_var = in_type;
    syntaxvalue_var = in_syntaxvalue;
    uint_var = in_uint;
    name_id_var = in_name_id;
}

void
 symbol::set_parameters(type in_type, syntaxvalue in_syntaxvalue, int in_uint,
			name in_name_id, vector <int> in_wvform)
{
    type_var = in_type;
    syntaxvalue_var = in_syntaxvalue;
    uint_var = in_uint;
    name_id_var = in_name_id;
	wvform_var = in_wvform;
}




/* Returns the type of symbol held in the object*/
type symbol::get_type()
{
    try {
	if (type_var != notype)
	    return type_var;
	else
	    throw get_symboldataexception_i;
    }
    catch(exception & e) {
	cout << e.what() << endl;
	throw;
	return notype;
    }
}

/* Returns the syntaxvalue of symbol held in the object (for
all except unames
*/
syntaxvalue symbol::get_syntaxvalue()
{
    return syntaxvalue_var;
}

/* returns uint if there is one */
int symbol::get_uint()
{
    return uint_var;
}

/* returns name id if there is one */
name symbol::get_name_id()
{
    return name_id_var;
}

// returns waveform in vector
vector <int> symbol::get_wvform()
{
	return wvform_var;
}


/* performs equality operation */
bool symbol::operator==(symbol rhs)
{
    if (type_var == rhs.get_type() &&
	syntaxvalue_var == rhs.get_syntaxvalue() &&
	uint_var == rhs.get_uint() && name_id_var == rhs.get_name_id())
	return true;

    return false;
}

/* performs inequality operation */
bool symbol::operator!=(symbol rhs)
{
    if (type_var == rhs.get_type() &&
	syntaxvalue_var == rhs.get_syntaxvalue() &&
	uint_var == rhs.get_uint() && name_id_var == rhs.get_name_id())
	return false;

    return true;
}

/* performs assignment operation */
symbol symbol::operator=(symbol rhs)
{
    type_var = rhs.get_type();
    syntaxvalue_var = rhs.get_syntaxvalue();
    uint_var = rhs.get_uint();
    name_id_var = rhs.get_name_id();
    return *this;
}

void symbol::uintdigits (vector <int> &digits, int int_in) // returns each digit in uint_in in a vector
{
	if (int_in > 9)
	{
		uintdigits (digits, int_in / 10);
	}

	digits.push_back (int_in % 10);
}
