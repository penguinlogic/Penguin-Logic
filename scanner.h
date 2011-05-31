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
int linenumber;
};

class scanner
{
private:
ifstream inf;
char curch;
character lastchar;
bool eofile;
namestring id;
int num;
int linenumber;
int charposition;
string currentline;

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
