#include "names.h"
#include <iostream>
#include <cstdlib>

using namespace std;

/* Name storage and retrieval routines */

/***************************************************************************************
names initialises the name table. This procedure is called at system initialisation before any of the above procedures/functions are used.
*/

names::names (void)		/* the constructor */
{
  vector < namestring > nametable;

}

/***************************************************************************************
Returns the internal representation of the name given in character form.  If the name is not already in the name table, it is automatically inserted.      
*/

name
names::lookup (namestring str)
{
  for (unsigned int i = 0; i < nametable.size (); i++)
    {
      if (!str.compare (nametable[i]))
	return i;
    }
  nametable.push_back (str);
  return nametable.size () - 1;
}

/***************************************************************************************
Returns the internal representation of the name given in character form.  If the name is not in the name table then 'blankname' is returned. 
*/

name
names::cvtname (namestring str)
{
  for (unsigned int i = 0; i < nametable.size (); i++)
    {
      if (!str.compare (nametable[i]))
	return i;
    }
  return -1;
}

/***************************************************************************************
 Prints out the given name on the console                            
*/

void
names::writename (name id)
{
  cout << nametable[id];
}

/***************************************************************************************
Does reverse lookup                          
*/

namestring
names::getname (name id)	

{
  return nametable[id];
}

/***************************************************************************************
Returns length ie number of characters in given name                
*/

int
names::namelength (name id)
{
  if (id < nametable.size ())
    return nametable[id].length ();
  else
    return -1;
}
