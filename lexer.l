%option noyywrap
%option c++

%{
#include "parser.tab.hh"
#include <cstdlib>
#include <string>
#include <iostream>
using namespace std;
%}

DIGIT   [0-9]
ID      [a-zA-Z_][a-zA-Z0-9_]*

%%

"print"     { return Parser::token::PRINT; }
"let"       { return Parser::token::LET; }

{ID}        { yylval->str = new std::string(yytext); return Parser::token::IDENT; }
{DIGIT}+    { yylval->num = atoi(yytext); return Parser::token::NUMBER; }

"="         { return '='; }
";"         { return ';'; }
"+"         { return '+'; }

[ \t\n]+    ;  // ignore whitespace
.           { cerr << "Caractère inconnu: " << yytext << endl; }
%%
