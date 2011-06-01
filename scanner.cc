#include "scanner.h"


scanner::scanner (names* names_mod, const char*defname)
{
inf.open(defname);
if(!inf) cout<<"unable to open file"<<endl;


else cout<<defname<<" was opened successfully"<<endl;
current.linenum=1;
current.pos=0;
getline(inf, current.line);
getch();
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
	//cout<<"line "<<current.linenum<<endl;
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
	getch();
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
 //cout<<"THE END"<<endl;
}

else if (isdigit(current.ch))
{	
	getnumber (num);
	s.set_parameters (Uint, novalue, num, 0);
}

else if (isalpha(current.ch))
{
	getname ( idstr);
	//cout << "idstr: " << idstr << endl;
	if (idstr=="DEVICES") s.set_parameters (Section, DEVICES, -1, names_mod.lookup(idstr)); //cout << "sgettype" << s.get_type() << endl;}
	else if (idstr=="CONNECTIONS")s.set_parameters (Section, CONNECTIONS, -1, names_mod.lookup(idstr));
	else if (idstr=="MONITOR") s.set_parameters (Section, MONITOR, -1, names_mod.lookup(idstr));
	
	else if (idstr=="CLOCK")s.set_parameters (Devname, CLOCK, -1, names_mod.lookup(idstr));
	else if (idstr=="SWITCH")s.set_parameters(Devname, SWITCH, -1, names_mod.lookup(idstr));
	else if (idstr=="AND")s.set_parameters (Devname, AND, -1, names_mod.lookup(idstr));
	else if (idstr=="NAND")s.set_parameters (Devname, NAND, -1, names_mod.lookup(idstr));
	else if (idstr=="OR")s.set_parameters(Devname, OR, -1, names_mod.lookup(idstr));
	else if (idstr=="NOR")s.set_parameters (Devname, NOR, -1, names_mod.lookup(idstr));
	else if (idstr=="DTYPE")s.set_parameters (Devname, DTYPE, -1, names_mod.lookup(idstr));
	else if (idstr=="XOR")s.set_parameters (Devname, XOR, -1, names_mod.lookup(idstr));

	else if (idstr=="DATA")s.set_parameters (Inname, DATA, -1, names_mod.lookup(idstr));
	else if (idstr=="SET")s.set_parameters (Inname, SET, -1, names_mod.lookup(idstr));
	else if (idstr=="CLEAR")s.set_parameters(Inname, CLEAR, -1, names_mod.lookup(idstr));
	else if (idstr=="CLK")s.set_parameters (Inname, CLK, -1, names_mod.lookup(idstr));
	else if (idstr=="I1")s.set_parameters (Inname, I1, -1, names_mod.lookup(idstr));
	else if (idstr=="I2")s.set_parameters (Inname, I2, -1, names_mod.lookup(idstr));
	else if (idstr=="I3")s.set_parameters (Inname, I3, -1, names_mod.lookup(idstr));
	else if (idstr=="I4")s.set_parameters (Inname, I4, -1, names_mod.lookup(idstr));
	else if (idstr=="I5")s.set_parameters(Inname, I5, -1, names_mod.lookup(idstr));
	else if (idstr=="I6")s.set_parameters(Inname, I6, -1, names_mod.lookup(idstr));
	else if (idstr=="I7")s.set_parameters(Inname, I7, -1, names_mod.lookup(idstr));
	else if (idstr=="I8")s.set_parameters(Inname, I8, -1, names_mod.lookup(idstr));
	else if (idstr=="I9")s.set_parameters(Inname, I9, -1, names_mod.lookup(idstr));
	else if (idstr=="I10")s.set_parameters(Inname, I10, -1, names_mod.lookup(idstr));
	else if (idstr=="I11")s.set_parameters(Inname, I11, -1, names_mod.lookup(idstr));
	else if (idstr=="I12")s.set_parameters(Inname, I12, -1, names_mod.lookup(idstr));
	else if (idstr=="I13")s.set_parameters(Inname, I13, -1, names_mod.lookup(idstr));
	else if (idstr=="I14")s.set_parameters(Inname, I14, -1, names_mod.lookup(idstr));
	else if (idstr=="I15")s.set_parameters(Inname, I15, -1, names_mod.lookup(idstr));
	else if (idstr=="I16")s.set_parameters(Inname, I16, -1, names_mod.lookup(idstr));

	else if (idstr=="period")s.set_parameters(Devswitch, period, -1, names_mod.lookup(idstr));
	else if (idstr=="initialvalue")s.set_parameters(Devswitch, initialvalue, -1, names_mod.lookup(idstr));
	else if (idstr=="numinputs") s.set_parameters(Devswitch, numinputs, -1, names_mod.lookup(idstr));

	else if (idstr=="Q")s.set_parameters (Outname, Q, -1, names_mod.lookup(idstr));
	else if (idstr=="QBAR") s.set_parameters(Outname, QBAR, -1, names_mod.lookup(idstr));

	else s.set_parameters (Uname, novalue, -1, names_mod.lookup(idstr));		
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
//cout<<"type: "<<s.get_type()<<" value:"<<s.get_value()<<" uint:"<<s.get_uint()<<" uname:"<<s.get_uname_id()<<endl;
}


void scanner::print_err(const char* error)
{
cout<<"Error in line "<<last.linenum<<", at character "<<last.pos<<":"<<endl;

cout<<last.line<<endl;

for (int i=0; i<last.pos-1; i++) {
	if (last.line[i] == '\t') cout << '\t';
	else cout << " ";
} // accounts for tabs at in erroneous line

cout<<"^"<<endl;
cout<<error<<endl;
}







