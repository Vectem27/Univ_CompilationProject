%{
#include "parser.tab.h"  // généré par Yacc/Bison
%}

%%

"print"     return PRINT;
[0-9]+      { yylval = atoi(yytext); return NUMBER; }
[ \t\n]+    ;
.           return *yytext;

%%
