#ifndef symbol_h
#define symbol_h
#include "names.h"
#include <iostream>
#include <string>

using namespace std;

  typedef enum {notype, Uint, Section, Devname, Devswitch, Outname, Inname, Charsym, Uname} type;
 // typedef enum {novalue, DEVICES, CONNECTIONS, MONITOR, CLOCK, SWITCH, AND, NAND, OR, NOR, DTYPE, XOR, period, initialvalue, numinputs, Q, QBAR, DATA, SET, CLEAR, CLK, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14, I15, I16, lbrak, rbrak, equals, rarrow, semicolon, dot, dash, endfile, bad} value;

class symbol
{
private:
  type type_var;
  int uint_var;
  name nametable_id;

 public:
  symbol ();

  void set_parameters (type in_type, name in_id, int in_uint);
  type get_type (); // returns symbol type
  value get_id (); // returns keyword values (i.e. not uints or unames)
  int get_uint (); // returns uint_var


  bool operator== (symbol rhs); // allows the use of ==
  bool operator!= (symbol rhs); // allows the use of !=
  symbol operator= (symbol rhs); // allows assignment

};

#endif // symbol_h

