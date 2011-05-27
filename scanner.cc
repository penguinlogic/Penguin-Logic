#include "scanner.h"


scanner::scanner (names* names_mod, const char*defname)
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


void scanner::getsymbol (symbol* s)
{
skipspaces();
skipcomments();

if (eofile) {
  s = new symbol (Charsym, eof, -1, "\0");
}

else if (isdigit(curch))
{
	
	getnumber (num);
	s = new symbol (Uint, novalue, num, "\0");
}

else if (isalpha(curch))
{
	getname ( id);
	if (id=="DEVICES") s = new symbol (Section, DEVICES, -1, "\0");
	if (id=="CONNECTIONS")s = new symbol (Section, CONNECTIONS, -1, "\0");
	if (id=="MONITOR") s = new symbol (Section, MONITOR, -1, "\0");
	if (id=="CLOCK")s = new symbol (Devname, CLOCK, -1, "\0");
	if (id=="SWITCH")s = new symbol (Devname, SWITCH, -1, "\0");
	if (id=="AND")s = new symbol (Devname, AND, -1, "\0");
	if (id=="NAND")s = new symbol (Devname, NAND, -1, "\0");
	if (id=="OR")s = new symbol (Devname, OR, -1, "\0");
	if (id=="NOR")s = new symbol (Devname, NOR, -1, "\0");
	if (id=="DTYPE")s = new symbol (Devname, DTYPE, -1, "\0");
	if (id=="XOR")s = new symbol (Devname, XOR, -1, "\0");
	if (id=="DATA")s = new symbol (Inname, DATA, -1, "\0");
	if (id=="SET")s = new symbol (Inname, SET, -1, "\0");
	if (id=="CLEAR")s = new symbol (Inname, CLEAR, -1, "\0");
	if (id=="CLOCK")s = new symbol (Inname, CLOCK, -1, "\0");
	if (id=="I1")s = new symbol (Inname, I1, -1, "\0");
	if (id=="I2")s = new symbol (Inname, I2, -1, "\0");
	if (id=="I3")s = new symbol (Inname, I3, -1, "\0");
	if (id=="I4")s = new symbol (Inname, I4, -1, "\0");
	if (id=="I5")s = new symbol (Inname, I5, -1, "\0");
	if (id=="I6")s = new symbol (Inname, I6, -1, "\0");
	if (id=="I7")s = new symbol (Inname, I7, -1, "\0");
	if (id=="I8")s = new symbol (Inname, I8, -1, "\0");
	if (id=="I9")s = new symbol (Inname, I9, -1, "\0");
	if (id=="I10")s = new symbol (Inname, I10, -1, "\0");
	if (id=="I11")s = new symbol (Inname, I11, -1, "\0");
	if (id=="I12")s = new symbol (Inname, I12, -1, "\0");
	if (id=="I13")s = new symbol (Inname, I13, -1, "\0");
	if (id=="I14")s = new symbol (Inname, I14, -1, "\0");
	if (id=="I15")s = new symbol (Inname, I15, -1, "\0");
	if (id=="I16")s = new symbol (Inname, I16, -1, "\0");
	if (id=="period")s = new symbol (Devswitch, period, -1, "\0");
	if (id=="initialvalue")s = new symbol (Devswitch, initialvalue, -1, "\0");
	if (id=="numinputs") s = new symbol (Devswitch, numinputs, -1, "\0");
	if (id=="Q")s = new symbol (Outname, Q, -1, "\0");
	if (id=="QBAR")s = new symbol (Outname, QBAR, -1, "\0");
	else s = new symbol (Uname, novalue, -1, id);
	
}

else
{
	switch (curch){ 
	case '.' : s=new symbol(Charsym, dot, -1, "\0"); break;
	case '{' : s=new symbol(Charsym, lbrak, -1, "\0"); break; 
	case '}' : s=new symbol(Charsym, rbrak, -1, "\0"); break;
	case '=' : s=new symbol(Charsym, equals, -1, "\0"); break;
	case ';' : s=new symbol(Charsym, semicolon, -1, "\0"); break;
	case '>' : s=new symbol(Charsym, rarrow, -1, "\0"); break;
	case '-' : s=new symbol(Charsym, dash, -1, "\0"); break;
	default: s = new symbol(Charsym, bad, -1, "\0"); break;	
	}
getch(); 
}
}










