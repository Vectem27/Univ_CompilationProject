%{
#include "Token.h"
using namespace std;

#define YY_NO_UNISTD_H

%}

%option noyywrap
%option c++


DIGIT   [0-9]
ID      [a-zA-Z_][a-zA-Z0-9_]*
FLOAT   ({DIGIT}+\.{DIGIT}+|{DIGIT}+\.|\.{DIGIT}+)
STRING  \"([^\\\"\n]|\\.)*\"

%%
"print"     { return TOK_PREDEF_FUNCTION; }

{ID}        { return TOK_IDENT; }
{STRING}    { return TOK_STRING; }
{FLOAT}     { return TOK_FLOAT; }
{DIGIT}+    { return TOK_INTEGER; }

"+"         { return TOK_PLUS; }
"-"         { return TOK_MINUS; }
"*"         { return TOK_STAR; }
"/"         { return TOK_SLASH; }
"%"         { return TOK_PERCENT; }
"="         { return TOK_EQUAL; }
"("         { return TOK_LPAREN; }
")"         { return TOK_RPAREN; }
";"         { return TOK_SEMICOLON; }



[ \t\n]+    ;
.           { std::cerr << "Unknown character: " << yytext << "\n"; }

%%