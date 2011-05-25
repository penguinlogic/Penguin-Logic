#ifndef symbol_h
#define symbol_h

using namespace std;

class symbol
{
 private:
  typedef enum type {'null' = -1, 'uint', 'bl', 'section', 'devname', 'devswitch', 'outname', 'inname', 'charsym', 'uname'} type_var = -1;

  int uint_var = -1;
  int bl_var = -1; // type is int so it can be initialised as null ('-1')
  typedef enum section {'null' = -1, 'DEVICES', 'CONNECTIONS', 'MONITOR'} section_var = -1;
  typedef enum devname {'null' = -1, 'CLOCK', 'SWITCH', 'AND', 'NAND', 'OR', 'NOR', 'DTYPE', 'XOR'} devname_var = -1;
  typedef enum devswitch {'null' = -1, '-period','-initialvalue', '-numinputs'} devswitch_var = -1;
  typedef enum outname {'null' = -1, 'Q', 'QBAR'} outname_var = -1;
  typedef enum inname {'null' = -1, 'DATA', 'SET', 'CLEAR', 'CLK', 'I1', 'I2', 'I3', 'I4', 'I5', 'I6', 'I7', 'I8', 'I9', 'I10', 'I11', 'I12', 'I13', 'I41', 'I15', 'I16'} inname_var = -1;
  typedef enum charsym {'null' = -1, '{', '}', '=', '>', ';', '.', 'eof'} charsym_var = -1;
  char* uname_var = '\0'; // null char

 public:
  symbol (int symtype, int num); // constructors
  symbol (int symtype, char* name_in);

  int get_type (); // returns symbol type

  int get_value (); // returns any value except uname_var
  char* get_uname (); // returns uname_var

  bool operator== (symbol rhs); // allows the use of ==

/* No method:
  bool isbad ();
as there should be no way for a symbol to be invalid.
Currently there is no way to change the value of a symbol;
values can only be given during initialisation.
*/

};

#endif // symbol_h

