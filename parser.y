%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
int yylex();

typedef struct {
    int num;
    char* str;
} YYSTYPE;

#define YYSTYPE YYSTYPE
%}

%union {
    int num;
    char* str;
}

%token <num> NUMBER
%token <str> IDENT
%token PRINT LET

%start program

%%

program:
      program stmt
    | stmt
    ;

stmt:
      PRINT NUMBER ';'        { printf("%d\n", $2); }
    | LET IDENT '=' NUMBER ';' { printf("Variable %s = %d\n", $2, $4); free($2); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erreur: %s\n", s);
}
