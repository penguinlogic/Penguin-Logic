#include "symbol.h"
int main()
{

symbol sym;
sym.set_parameters(Uint, novalue, 68, "\0");
cout<<"flag"<<endl;
cout<<"type:"<<sym.get_type()<<" value:"<<sym.get_value()<<" uint:"<<sym.get_uint()<<" uname:"<<sym.get_uname()<<endl;
}
