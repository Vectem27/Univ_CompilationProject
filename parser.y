%code requires {
    #include "ast.h"
}


%{
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "ast.h"

std::vector<std::unique_ptr<Stmt>> ast;
void yyerror(const char* s);
int yylex(void);

//void yyerror(const char* s) { std::cerr << "Erreur: " << s << "\n"; }
%}

%union {
    int num;
    std::string* str;
    Stmt* stmt;
}

%token <num> NUMBER
%token <str> IDENT
%token PRINT LET

%type <stmt> stmt

%start program

%%

program:
      program stmt   { ast.emplace_back($2); }
    | stmt           { ast.emplace_back($1); }
    ;

stmt:
      PRINT NUMBER ';'        { $$ = new PrintStmt($2); }
    | LET IDENT '=' NUMBER ';' { $$ = new LetStmt(*$2, $4); delete $2; }
    ;

%%