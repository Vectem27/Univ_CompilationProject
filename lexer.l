%{
#include "parser.tab.h"   // header généré par Bison
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

%}

DIGIT   [0-9]
ID      [a-zA-Z_][a-zA-Z0-9_]*

%%

"print"     { return PRINT; }
"let"       { return LET; }

{ID}        { yylval.str = strdup(yytext); return IDENT; }
{DIGIT}+    { yylval.num = atoi(yytext); return NUMBER; }

"="         { return '='; }
";"         { return ';'; }
"+"         { return '+'; }

[ \t\n]+    ;  // ignore whitespace
.           { fprintf(stderr, "Caractère inconnu: %s\n", yytext); }
%%

int yywrap(void) { return 1; }
