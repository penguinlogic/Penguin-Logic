#ifndef symbol_h
#define symbol_h
#include "names.h"
#include <iostream>
#include <string>

using namespace std;

typedef enum
    { notype, Uint, Section, Devname, Devswitch, Outname, Inname, Charsym,
	Uname
} type;
typedef enum
    { novalue, DEVICES, CONNECTIONS, MONITOR, period, initialvalue, numinputs,
   waveform,
	lbrak, rbrak, equals, rarrow, semicolon, dot, dash, endfile,
	bad
} syntaxvalue;

class symbol {
 private:
	type type_var;
	syntaxvalue syntaxvalue_var;
	int uint_var;
	name name_id_var;
	 vector < int >wvform_var;

 public:
	 symbol();

	void set_parameters(type in_type, syntaxvalue in_syntaxvalue,
			    int in_uint, name in_name_id);
	void set_parameters(type in_type, syntaxvalue in_syntaxvalue,
			    int in_uint, name in_name_id,
			    vector < int >in_wform);
	type get_type();	// returns symbol type
	syntaxvalue get_syntaxvalue();	// returns keyword values (i.e. not uints or unames)
	int get_uint();		// returns uint_var
	name get_name_id();	// returns uname__id_var
	 vector < int >get_wvform();	// returns waveform in a vector

	bool operator==(symbol rhs);	// allows the use of ==
	bool operator!=(symbol rhs);	// allows the use of !=
	symbol operator=(symbol rhs);	// allows assignment

	void uintdigits(vector < int >&digits, int int_in);	// puts each digit in uint_in into a vector

};

#endif				// symbol_h
