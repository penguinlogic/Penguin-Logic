#include "scanner.h"
#include "names.h"
#include "symbol.h"
names testnames;
char filename[] = "grammar_eg1.txt";
scanner scan(&testnames, filename);

int main()
{
symbol sym;
for (int i=0; i<10; i++)
{
scan.getsymbol(sym);
cout<<"Symbol "<<i<<" type: "<<sym.get_type()<<" value:"<<sym.get_value()<<" uint:"<<sym.get_uint()<<" uname:"<<sym.get_uname()<<endl;
}
}

