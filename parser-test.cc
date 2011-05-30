#include <iostream>
#include "parser.h"
#include "scanner.h"
#include "symbol.h"

int main() {
names testnames;
char filename[] = "grammar_eg1.txt";
scanner scan(&testnames, filename);
parser p(&scan);
p.readin();

}



