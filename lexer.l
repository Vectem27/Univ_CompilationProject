%{
#include "Token.h"
using namespace std;

#define YY_NO_UNISTD_H

%}

%option noyywrap
%option c++


DIGIT   [0-9]
ID      [a-zA-Z_][a-zA-Z0-9_]*

%%
{ID}        { ; }
{DIGIT}+    { return TOK_INTEGER; }

"+"         { return TOK_PLUS; }

[ \t\n]+    ;
.           { std::cerr << "Unknown character: " << yytext << "\n"; }

%%