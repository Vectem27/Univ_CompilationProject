%{
#include "parser.tab.hh"
#include <cstdlib>
#include <string>
#include <FlexLexer.h>
using namespace std;

#define YY_NO_UNISTD_H

%}

%option noyywrap
%option c++


DIGIT   [0-9]
ID      [a-zA-Z_][a-zA-Z0-9_]*

%%

"print"     { ;/*return PRINT; */}
"let"       { ;/*return LET; */}

{ID}        { ;/*yylval->str = new std::string(yytext); return IDENT;*/ }
{DIGIT}+    { ;/*yylval->num = atoi(yytext); return NUMBER;*/ }

"="         { return '='; }
";"         { return ';'; }
"+"         { return '+'; }

[ \t\n]+    ;  // ignore whitespace
.           { std::cerr << "Caractère inconnu: " << yytext << "\n"; }

%%