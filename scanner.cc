#include "scanner.h"


scanner::scanner (names* names_mod, const char*defname)
{
inf.open(defname);
if(!inf) cout<<"unable to open file"<<endl;
else cout<<defname<<" was opened successfully"<<endl;
linenumber=1;
charposition=0;
getline(inf, currentline);
cout<<"line "<<linenumber<<endl;
getch();
}


void scanner::getch()
{
last.ch=curch;
last.pos=charposition;
last.linenumber=linenumber;
last.line=currentline;

if (!inf.eof())
{
if (charposition==currentline.size())
{
getline(inf, currentline);
charposition=0;
linenumber++;
cout<<"line "<<linenumber<<endl;
}

if(charposition<=(currentline.size()-1)){
curch=currentline[charposition];
charposition++;
}

}
}


void scanner::getnumber (int &number)
{
  number =0;
  string string;
  while (!inf.eof() && isdigit(curch))
    {
      string.push_back(curch);
      stringstream ss(string);
   	ss >>number;
	getch();
    }
}

void scanner::skipspaces ()
{

while (currentline.size()==0 && !inf.eof())
{
getline(inf, currentline);
charposition=0;
linenumber++;
cout<<"line "<<linenumber<<endl;
} 

  while (!inf.eof() && isspace(curch))
   {
	cout<<"*"<<endl;
   getch();
   }
}

void scanner::skipcomments()
{
if (curch=='/')
{
do {getch();} while (curch !='/' && !inf.eof());
getch();
skipspaces();
}
}

void scanner::getname (namestring &str)
{
	str.clear();
	while (!inf.eof() && (isalnum(curch))){
	str.push_back(curch);	
	getch();
	} 

}


void scanner::getsymbol (symbol& s)
{
skipspaces();
skipcomments();

if (inf.eof()) {
  s.set_parameters (Charsym, endfile, -1, "\0");
 cout<<"THE END"<<endl;
}

else if (isdigit(curch))
{	
	getnumber (num);
	s.set_parameters (Uint, novalue, num, "\0");
}

else if (isalpha(curch))
{
	getname ( id);
	cout << "id: " << id << endl;
	if (id=="DEVICES") s.set_parameters (Section, DEVICES, -1, "\0"); //cout << "sgettype" << s.get_type() << endl;}
	else if (id=="CONNECTIONS")s.set_parameters (Section, CONNECTIONS, -1, "\0");
	else if (id=="MONITOR") s.set_parameters (Section, MONITOR, -1, "\0");
	else if (id=="CLOCK")s.set_parameters (Devname, CLOCK, -1, "\0");
	else if (id=="SWITCH")s.set_parameters(Devname, SWITCH, -1, "\0");
	else if (id=="AND")s.set_parameters (Devname, AND, -1, "\0");
	else if (id=="NAND")s.set_parameters (Devname, NAND, -1, "\0");
	else if (id=="OR")s.set_parameters(Devname, OR, -1, "\0");
	else if (id=="NOR")s.set_parameters (Devname, NOR, -1, "\0");
	else if (id=="DTYPE")s.set_parameters (Devname, DTYPE, -1, "\0");
	else if (id=="XOR")s.set_parameters (Devname, XOR, -1, "\0");
	else if (id=="DATA")s.set_parameters (Inname, DATA, -1, "\0");
	else if (id=="SET")s.set_parameters (Inname, SET, -1, "\0");
	else if (id=="CLEAR")s.set_parameters(Inname, CLEAR, -1, "\0");
	else if (id=="CLOCK")s.set_parameters (Inname, CLOCK, -1, "\0");
	else if (id=="I1")s.set_parameters (Inname, I1, -1, "\0");
	else if (id=="I2")s.set_parameters (Inname, I2, -1, "\0");
	else if (id=="I3")s.set_parameters (Inname, I3, -1, "\0");
	else if (id=="I4")s.set_parameters (Inname, I4, -1, "\0");
	else if (id=="I5")s.set_parameters(Inname, I5, -1, "\0");
	else if (id=="I6")s.set_parameters(Inname, I6, -1, "\0");
	else if (id=="I7")s.set_parameters(Inname, I7, -1, "\0");
	else if (id=="I8")s.set_parameters(Inname, I8, -1, "\0");
	else if (id=="I9")s.set_parameters(Inname, I9, -1, "\0");
	else if (id=="I10")s.set_parameters(Inname, I10, -1, "\0");
	else if (id=="I11")s.set_parameters(Inname, I11, -1, "\0");
	else if (id=="I12")s.set_parameters(Inname, I12, -1, "\0");
	else if (id=="I13")s.set_parameters(Inname, I13, -1, "\0");
	else if (id=="I14")s.set_parameters(Inname, I14, -1, "\0");
	else if (id=="I15")s.set_parameters(Inname, I15, -1, "\0");
	else if (id=="I16")s.set_parameters(Inname, I16, -1, "\0");
	else if (id=="period")s.set_parameters(Devswitch, period, -1, "\0");
	else if (id=="initialvalue")s.set_parameters(Devswitch, initialvalue, -1, "\0");
	else if (id=="numinputs") s.set_parameters(Devswitch, numinputs, -1, "\0");
	else if (id=="Q")s.set_parameters (Outname, Q, -1, "\0");
	else if (id=="QBAR")s.set_parameters(Outname, QBAR, -1, "\0");
	else s.set_parameters (Uname, novalue, -1, id);
	
}

else
{
	switch (curch){ 
	case '.' : s.set_parameters(Charsym, dot, -1, "\0"); break;
	case '{' : s.set_parameters(Charsym, lbrak, -1, "\0"); break; 
	case '}' : s.set_parameters(Charsym, rbrak, -1, "\0"); break;
	case '=' : s.set_parameters(Charsym, equals, -1, "\0"); break;
	case ';' : s.set_parameters(Charsym, semicolon, -1, "\0"); break;
	case '>' : s.set_parameters(Charsym, rarrow, -1, "\0"); break;
	case '-' : s.set_parameters(Charsym, dash, -1, "\0"); break;
	default: s.set_parameters(Charsym, bad, -1, "\0"); break;	
	}
getch(); 
}
cout<<"type: "<<s.get_type()<<" value:"<<s.get_value()<<" uint:"<<s.get_uint()<<" uname:"<<s.get_uname()<<endl;
}


void scanner::print_err(const char* error)
{
cout<<"Error in line "<<lastchar.linenumber<<", at character "<<lastchar.charposition<<":"<<endl;
cout<<lastchar.line<<endl;
for (int i=0; i<lastchar.charposition-1; i++) cout<<" ";
cout<<"^"<<endl;
cout<<error<<endl;
}







