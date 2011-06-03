#include "scanner.h"

/***************************************************************************************/
scanner::scanner (names* names_mod, const char*defname)
{
inf.open(defname);
if(!inf) cout<<"unable to open file"<<endl;


else cout<<defname<<" was opened successfully"<<endl;
nmz=names_mod;
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

	if(current.pos<(current.line.size())){
	current.ch=current.line[current.pos];
	current.pos++;
	}
}
}
/***************************************************************************************/

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
/***************************************************************************************/
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
/***************************************************************************************/
void scanner::skipcomments()
{
if (current.ch=='/')
{
	do {getch();} while (current.ch !='/' && !inf.eof());
	getch();
	skipspaces();
}
}
/***************************************************************************************/
void scanner::getname (namestring &str)
{
	str.clear();
	while (!inf.eof() && (isalnum(current.ch))){
	str.push_back(current.ch);	
	getch();
	} 

}

/***************************************************************************************/
void scanner::getsymbol (symbol& s)
{
skipspaces();
skipcomments();

if (inf.eof()) {
  s.set_parameters (Charsym, endfile, -1, -1);
 //cout<<"THE END"<<endl;
}

else if (isdigit(current.ch))
{	
	getnumber (num);
	s.set_parameters (Uint, novalue, num, -1);
}

else if (isalpha(current.ch))
{
getname (idstr);

//cout << "idstr: " << idstr << endl;

if (idstr=="DEVICES") s.set_parameters (Section, DEVICES, -1, -1);
else if (idstr=="CONNECTIONS") s.set_parameters (Section, CONNECTIONS, -1, -1);
else if (idstr=="MONITOR") s.set_parameters (Section, MONITOR, -1, -1);

else if (idstr=="period") s.set_parameters(Devswitch, period, -1, -1);
else if (idstr=="initialvalue")s.set_parameters(Devswitch, initialvalue, -1, -1);
else if (idstr=="numinputs") s.set_parameters(Devswitch, numinputs, -1, -1);

else if (idstr=="CLOCK"||idstr=="SWITCH"||idstr=="AND"||idstr=="NAND"||idstr=="OR"||idstr=="NOR"||idstr=="DTYPE"||idstr=="XOR") s.set_parameters (Devname, novalue, -1, nmz->lookup(idstr));

else if (idstr=="DATA"||idstr=="SET"||idstr=="CLEAR"||idstr=="CLK"||idstr=="I1"||idstr=="I2"||idstr=="I3"||idstr=="I4"||idstr=="I5"||idstr=="I6"||idstr=="I7"||idstr=="I8"||idstr=="I9"||idstr=="I10"||idstr=="I11"||idstr=="I12"||idstr=="I13"||idstr=="I14"||idstr=="I15"||idstr=="I16") s.set_parameters (Inname, novalue, -1, nmz->lookup(idstr));

else if (idstr=="Q"||idstr=="QBAR") s.set_parameters (Outname, novalue, -1, nmz->lookup(idstr));

else s.set_parameters (Uname, novalue, -1, nmz->lookup(idstr));	
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

/***************************************************************************************/
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







