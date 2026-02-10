%{
#include <stdio.h>
#include <stdlib.h>
%}

%token PRINT NUMBER

%%

stmt: PRINT NUMBER ';' { printf("Print number: %d\n", $2); }
    ;

%%

int main() {
    yyparse();
    return 0;
}

int yyerror(char *s) {
    fprintf(stderr, "Erreur syntaxe: %s\n", s);
    return 0;
}
