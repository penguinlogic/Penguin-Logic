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
	// Insert private stuff here
ifstream inf;
char curch;
bool eofile;
char * id;

public:

	
	
	scanner(names* names_mod, char* defname); // Constructor

	//~scanner();		//Destructor

	void getsymbol (symbol &s, namestring &id, int &num); //return type of symbol, name if it has one, value if a number.
	void skipspaces ();
	void skipcomments();
	void getch();
	void getnumber (int &number);
	void getname (namestring &str);

};

#endif /* scanner_h */
