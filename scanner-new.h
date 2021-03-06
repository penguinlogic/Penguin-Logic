#ifndef scanner_h
#define scanner_h
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <string>
#include <sstream>
#include <vector>
#include "symbol.h"
#define stringify( name ) # name

//#include <wx/string.h>
#include "names.h"

using namespace std;

struct character
{public:
string line;
char ch;
int pos;
int linenum;
};

class scanner
{
private:
names names_mod;
ifstream inf;
character current;
character last;
bool eofile;
namestring idstr;
int num;

public:	
scanner(names* names_mod, const char* defname); // Constructor

void initialise_names_mod(void);
void getsymbol (symbol &s); //gets next symbol object
void skipspaces ();
void skipcomments();
void getch();
void getnumber (int &number);
void getname (namestring &str);
void print_err(const char* error);

};



#endif /* scanner_h */
