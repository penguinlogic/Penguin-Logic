#include "scanner.h"


scanner::scanner (names* names_mod, char*defname)
{
inf.open(defname);
if(!inf) cout<<"unable to open file"<<endl;
else cout<<defname<<" was opened successfully"<<endl;
getch();
}


void scanner::getch()
{
curch=0;
eofile=(inf.get(curch)==0);
}


void scanner::getnumber (int &number)
{
  number =0;
  string string;
  while (!eofile && isdigit(curch))
    {
      string.push_back(curch);
      stringstream ss(string);
   	ss >>number;
	getch();
    }
}

void scanner::skipspaces ()
{
 
  while (!eofile && isspace(curch))
   {
   getch();
   }
}

void scanner::skipcomments()
{
if (curch=='/')
{
do {getch();} while (curch !='/');
getch();
skipspaces();
}
}

void scanner::getname (namestring &str)
{
	str.clear();
	while (!eofile && (isalnum(curch))) 
	{
	str.push_back(curch);
	getch();
	}

}


void scanner::getsymbol (symbol &s)
{
skipspaces();
skipcomments();

if (eofile) {
  s = new symbol (7,7);
}

else if (isdigit(curch))
{
	
	getnumber (num);
	s = new symbol (0,num);
}

else if (isalpha(curch))
{
	getname ( id);
	if (id=="DEVICES") s = new symbol (2,0);

	if (id=="CONNECTIONS")s = new symbol (2,1);
	if (id=="MONITORS") s = new symbol (2,2);
	if (id=="CLOCK")s = new symbol (3,0);
	if (id=="SWITCH")s = new symbol (3,1);
	if (id=="AND")s = new symbol (3,2);
	if (id=="NAND")s = new symbol (3,3);
	if (id=="OR")s = new symbol (3,4);
	if (id=="NOR")s = new symbol (3,5);
	if (id=="DTYPE")s = new symbol (3,6);
	if (id=="XOR")s = new symbol (3,7);
	if (id=="DATA")s = new symbol (6,0);
	if (id=="SET")s = new symbol (6,1);
	if (id=="CLEAR")s = new symbol (6,2);
	if (id=="CLOCK")s = new symbol (6,3);
	if (id=="I1")s = new symbol (6,4);
	if (id=="I2")s = new symbol (6,5);
	if (id=="I3")s = new symbol (6,6);
	if (id=="I4")s = new symbol (6,7);
	if (id=="I5")s = new symbol (6,8);
	if (id=="I6")s = new symbol (6,9);
	if (id=="I7")s = new symbol (6,10);
	if (id=="I8")s = new symbol (6,11);
	if (id=="I9")s = new symbol (6,12);
	if (id=="I10")s = new symbol (6,13);
	if (id=="I11")s = new symbol (6,14);
	if (id=="I12")s = new symbol (6,15);
	if (id=="I13")s = new symbol (6,16);
	if (id=="I14")s = new symbol (6,17);
	if (id=="I15")s = new symbol (6,18);
	if (id=="I16")s = new symbol (6,19);
	if (id=="period")s = new symbol (4,0);
	if (id=="initialvalue")s = new symbol (4,1);
	if id=="numinputs") s = new symbol (4,2);
	if (id=="Q")s = new symbol (5,0);
	if (id=="QBAR")s = new symbol (5,1);
	else s = new symbol (8, id);
	
}

else
{
	switch (curch){ 
	case '.' : s=new symbol(7,5); break;
	case '{' : s=new symbol(7,0); break; 
	case '}' : s=new symbol(7,1); break;
	case '=' : s=new symbol(7,2); break;
	case ';' : s=new symbol(7,4); break;
	case '>' : s=new symbol(7,3); break;
	case '-' : s=new symbol(7,6); break;
	default: s = new symbol(7,8); break;	
	}
getch(); 
}
}










