#ifndef symbol_h
#define symbol_h

#include <iostream>
#include <string>

using namespace std;

  typedef enum {notype = -1, Uint, Section, Devname, Devswitch, Outname, Inname, Charsym, Uname} type;
  typedef enum {novalue = -1, DEVICES, CONNECTIONS, MONITOR, CLOCK, SWITCH, AND, NAND, OR, NOR, DTYPE, XOR, period, initialvalue, numinputs, Q, QBAR, DATA, SET, CLEAR, CLK, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14, I15, I16, lbrak, rbrak, equals, rarrow, semicolon, dot, dash, endfile, bad} value;

class symbol
{
private:
  type type_var;
  value value_var;
  int uint_var;
  string uname_var;

 public:
  symbol ();
  symbol (type in_type, value in_value, int in_uint, string in_uname); // constructor
  //symbol (int symtype, string name_in);

  type get_type (); // returns symbol type
  value get_value (); // returns keyword values (i.e. not uints or unames)
  int get_uint (); // returns uint_var
  string get_uname (); // returns uname_var

  bool operator== (symbol rhs); // allows the use of ==
  bool operator!= (symbol rhs); // allows the use of !=
  symbol operator= (symbol rhs); // allows assignment

/* No method:
  bool isbad ();
as there should be no way for a symbol to be invalid.
Currently there is no way to change the value of a symbol;
values can only be given during initialisation.
*/

};

#endif // symbol_h

