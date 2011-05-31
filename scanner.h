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

class character
{public:
string line;
char ch;
int pos;
int linenum;
character operator= (character rhs); // allows assignment
};

class scanner
{
private:
ifstream inf;
character current;
character last;
bool eofile;
namestring id;
int num;

public:	
scanner(names* names_mod, const char* defname); // Constructor

void getsymbol (symbol &s); //gets next symbol object
void skipspaces ();
void skipcomments();
void getch();
void getnumber (int &number);
void getname (namestring &str);
void print_err(const char* error);

};



#endif /* scanner_h */
