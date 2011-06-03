#include "scanner.h"


scanner::scanner (names* names_mod, const char*defname)
{
inf.open(defname);
if(!inf) cout<<"unable to open file"<<endl;
else cout<<defname<<" was opened successfully"<<endl;
initialise_names_mod();
current.linenum=1;
current.pos=0;
getline(inf, current.line);
getch();
}

/* performs assignment operation */
character character::operator= (character rhs) {
  ch = rhs.ch;
  pos = rhs.pos;
  linenum = rhs.linenum;
  line = rhs.line;
  return *this;
}

void scanner::initialise_names_mod(void)
{

names_mod.lookup("novalue"); names_mod.lookup("DEVICES"); names_mod.lookup("CONNECTIONS"); names_mod.lookup("MONITOR"); names_mod.lookup("CLOCK"); names_mod.lookup("SWITCH"); names_mod.lookup("AND"); names_mod.lookup("NAND"); names_mod.lookup("OR"); names_mod.lookup("NOR"); names_mod.lookup("DTYPE"); names_mod.lookup("XOR"); names_mod.lookup("period"); names_mod.lookup("initialvalue"); names_mod.lookup("numinputs"); names_mod.lookup("Q"); names_mod.lookup("QBAR"); names_mod.lookup("DATA"); names_mod.lookup("SET"); names_mod.lookup("CLEAR"); names_mod.lookup("CLK"); names_mod.lookup("I1"); names_mod.lookup("I2"); names_mod.lookup("I3"); names_mod.lookup("I4"); names_mod.lookup("I5"); names_mod.lookup("I6"); names_mod.lookup("I7"); names_mod.lookup("I8"); names_mod.lookup("I9"); names_mod.lookup("I10"); names_mod.lookup("I11"); names_mod.lookup("I12"); names_mod.lookup("I13"); names_mod.lookup("I14"); names_mod.lookup("I15"); names_mod.lookup("I16"); names_mod.lookup("lbrak"); names_mod.lookup("rbrak"); names_mod.lookup("equals"); names_mod.lookup("rarrow"); names_mod.lookup("semicolon"); names_mod.lookup("dot"); names_mod.lookup("dash"); names_mod.lookup("endfile"); names_mod.lookup("bad");
}

void scanner::getch()
{
last=current;

if (!inf.eof())
{
	if (current.pos==current.line.size())
	{
	getline(inf, current.line);
	current.pos=0;
	current.linenum++;
	cout<<"line "<<current.linenum<<endl;
	}

	if(current.pos<=(current.line.size()-1)){
	current.ch=current.line[current.pos];
	current.pos++;
	}
}
}


void scanner::getnumber (int &number)
{
  number =0;
  string str;
  while (!inf.eof() && isdigit(current.ch))
    {
      str.push_back(current.ch);
      stringstream ss(str);
   	ss >>number;
	getch();
    }
}

void scanner::skipspaces ()
{

while (current.line.size()==0 && !inf.eof())
{
	getline(inf, current.line);
	current.pos=0;
	current.linenum++;
} 

while (!inf.eof() && isspace(current.ch)) getch();
}

void scanner::skipcomments()
{
if (current.ch=='/')
{
	do {getch();} while (current.ch !='/' && !inf.eof());
	getch();
	skipspaces();
}
}

void scanner::getname (namestring &str)
{
	str.clear();
	while (!inf.eof() && (isalnum(current.ch))){
	str.push_back(current.ch);	
	getch();
	} 

}


void scanner::getsymbol (symbol& s)
{
skipspaces();
skipcomments();

if (inf.eof()) {
  s.set_parameters (Charsym, endfile, -1, 0);
 cout<<"THE END"<<endl;
}

else if (isdigit(current.ch))
{	
	getnumber (num);
	s.set_parameters (Uint, novalue, num, 0);
}

else if (isalpha(current.ch))
{
	getname ( idstr);
	cout << "idstr: " << idstr << endl;
	if (idstr=="DEVICES") s.set_parameters (Section, names_mod.lookup(idstr), -1); //cout << "sgettype" << s.get_type() << endl;}
	else if (idstr=="CONNECTIONS")s.set_parameters (Section, names_mod.lookup(idstr), -1);
	else if (idstr=="MONITOR") s.set_parameters (Section, names_mod.lookup(idstr), -1);






else if (idstr=="CLOCK")s.set_parameters (Devname, names_mod.lookup(idstr), -1);
	else if (idstr=="SWITCH")s.set_parameters(Devname, names_mod.lookup(idstr), -1);
	else if (idstr=="AND")s.set_parameters (Devname, names_mod.lookup(idstr), -1);
	else if (idstr=="NAND")s.set_parameters (Devname, names_mod.lookup(idstr), -1);
	else if (idstr=="OR")s.set_parameters(Devname, names_mod.lookup(idstr), -1);
	else if (idstr=="NOR")s.set_parameters (Devname,names_mod.lookup(idstr), -1);
	else if (idstr=="DTYPE")s.set_parameters (Devname, names_mod.lookup(idstr), -1);
	else if (idstr=="XOR")s.set_parameters (Devname, names_mod.lookup(idstr), -1);

else if (idstr=="DATA")s.set_parameters (Inname, names_mod.lookup(idstr), -1);
	else if (idstr=="SET")s.set_parameters (Inname, names_mod.lookup(idstr), -1);
	else if (idstr=="CLEAR")s.set_parameters(Inname, names_mod.lookup(idstr), -1);
	else if (idstr=="CLOCK")s.set_parameters (Inname, names_mod.lookup(idstr), -1);
	else if (idstr=="I1")s.set_parameters (Inname, names_mod.lookup(idstr), -1);
	else if (idstr=="I2")s.set_parameters (Inname, names_mod.lookup(idstr), -1);
	else if (idstr=="I3")s.set_parameters (Inname, names_mod.lookup(idstr), -1);
	else if (idstr=="I4")s.set_parameters (Inname, names_mod.lookup(idstr), -1);
	else if (idstr=="I5")s.set_parameters(Inname, names_mod.lookup(idstr), -1);
	else if (idstr=="I6")s.set_parameters(Inname, names_mod.lookup(idstr), -1);
	else if (idstr=="I7")s.set_parameters(Inname, names_mod.lookup(idstr), -1);
	else if (idstr=="I8")s.set_parameters(Inname, names_mod.lookup(idstr), -1);
	else if (idstr=="I9")s.set_parameters(Inname, names_mod.lookup(idstr), -1);
	else if (idstr=="I10")s.set_parameters(Inname, names_mod.lookup(idstr), -1);
	else if (idstr=="I11")s.set_parameters(Inname, names_mod.lookup(idstr), -1);
	else if (idstr=="I12")s.set_parameters(Inname, names_mod.lookup(idstr), -1);
	else if (idstr=="I13")s.set_parameters(Inname, names_mod.lookup(idstr), -1);
	else if (idstr=="I14")s.set_parameters(Inname, names_mod.lookup(idstr), -1);
	else if (idstr=="I15")s.set_parameters(Inname, names_mod.lookup(idstr), -1);
	else if (idstr=="I16")s.set_parameters(Inname, names_mod.lookup(idstr), -1);

else if (idstr=="period")s.set_parameters(Devswitch, names_mod.lookup(idstr), -1);
	else if (idstr=="initialvalue")s.set_parameters(Devswitch, names_mod.lookup(idstr), -1);
	else if (idstr=="numinputs") s.set_parameters(Devswitch, names_mod.lookup(idstr), -1);


else if (idstr=="Q")s.set_parameters (Outname, names_mod.lookup(idstr), -1);
	else if (idstr=="QBAR")s.set_parameters(Outname, names_mod.lookup(idstr), -1);

else s.set_parameters (Uname, names_mod.lookup(idstr), -1);
}		
}

else
{
	switch (current.ch){ 
	case '.' : s.set_parameters(Charsym, dot, -1, 0); break;
	case '{' : s.set_parameters(Charsym, lbrak, -1, 0); break; 
	case '}' : s.set_parameters(Charsym, rbrak, -1, 0); break;
	case '=' : s.set_parameters(Charsym, equals, -1, 0); break;
	case ';' : s.set_parameters(Charsym, semicolon, -1, 0); break;
	case '>' : s.set_parameters(Charsym, rarrow, -1, 0); break;
	case '-' : s.set_parameters(Charsym, dash, -1, 0); break;
	default: s.set_parameters(Charsym, bad, -1, 0); break;	
	}
getch(); 
}
cout<<"type: "<<s.get_type()<<" value:"<<s.get_value()<<" uint:"<<s.get_uint()<<" uname:"<<s.get_uname_id()<<endl;
}


void scanner::print_err(const char* error)
{
cout<<"Error in line "<<last.linenum<<", at character "<<last.pos<<":"<<endl;
cout<<last.line<<endl;
for (int i=0; i<last.pos-1; i++) cout<<" ";
cout<<"^"<<endl;
cout<<error<<endl;
}
