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

#include <wx/string.h>
#include "names.h"

using namespace std;

/***************************************************************************
Contains the character and, to enable printing of errors, includes its line number, column position and a string containing the line itself
*/

struct character {
 public:
	string line;
	char ch;
	unsigned int pos;
	int linenum;
};

/***************************************************************************/
class scanner {
 private:
	names * nmz;
	ifstream inf;
	character current;
	character last;
	bool eofile;
	namestring idstr;
	int num;
	void skipspaces();
	void skipcomments();
	void getch();
	void getnumber(int &number);
	void getnumber(int &number, vector < int >&wvform);
	void getname(namestring & str);

 public:
	 scanner(names * names_mod, const char *defname, bool & ok);	// Constructor
	void getsymbol(symbol & s);	//gets next symbol object
	void print_err(const char *error);

};

#endif				/* scanner_h */
