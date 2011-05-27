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

class scanner
{
private:
ifstream inf;
char curch;
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

};

#endif /* scanner_h */
