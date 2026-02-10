%token NUMBER IDENT PRINT LET
%start program

%%

program:
      program stmt
    | stmt
    ;

stmt:
      PRINT NUMBER ';' { cout << yylval_num << endl; }
    | LET IDENT '=' NUMBER ';' { cout << "Variable " << yylval_str << " = " << yylval_num << endl; }
    ;
%%
