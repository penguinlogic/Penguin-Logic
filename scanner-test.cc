#include "scanner.h"
#include "names.h"
#include "symbol.h"
names testnames;
char filename[] = "grammar_eg1.txt";
scanner scan(&testnames, filename);

int main()
{
symbol sym;
while (sym.get_value()!=endfile)
//for (int i=0; i<10; i++)
{
scan.getsymbol(sym);
cout<<"type: "<<sym.get_type()<<" value:"<<sym.get_value()<<" uint:"<<sym.get_uint()<<" uname:"<<sym.get_uname_id()<<endl;

if (sym.get_value() == MONITOR){
char error[]= "exception";
scan.print_err(error);
break;

}
}
}
