#include "names.h"
#include <iostream>
#include <cstdlib>

using namespace std;


/* Name storage and retrieval routines */

names::names(void)  /* the constructor */	/* names initialises the name table.  This procedure is called at      */
    						/* system initialisation before any of the above procedures/functions  */
    						/* are used.                                                           */
{
vector<namestring> nametable;

}

name names::lookup (namestring str) 	/* Returns the internal representation of the name given in character  */
    					/* form.  If the name is not already in the name table, it is          */
    					/* automatically inserted.      */
 {
for (unsigned int i=0; i<nametable.size(); i++)
	{
	if (!str.compare(nametable[i])) return i;
	}
nametable.push_back(str);
return nametable.size()-1;
}


name names::cvtname (namestring str)	/* Returns the internal representation of the name given in character  */
    					/* form.  If the name is not in the name table then 'blankname' is     */
    					/* returned. */
 {
for (unsigned int i=0; i<nametable.size(); i++)
	{
	if (!str.compare(nametable[i])) return i;
	}
return -1;
}

void names::writename (name id)		/* Prints out the given name on the console                            */
{
cout<< nametable[id];
}

namestring names::getname(name id) /* Does reverse lookup */
{
	return nametable[id];
}


int names::namelength (name id)		 /* Returns length ie number of characters in given name                */
{
	if(id<nametable.size())
		return nametable[id].length();
	else
		return -1;
}