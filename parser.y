%require "3.8"
%language "c++"
%define api.value.type variant
%define api.token.constructor
%define api.token.raw

%code requires {
    #include "Ast.h"
    #include <memory>
    #include <string>
    #include <vector>
    #include <iostream>

    class LexerWrapper;
}

%parse-param { LexerWrapper& lexer }
%lex-param { LexerWrapper& lexer }

%code {
    #include "LexerWrapper.h"

    extern std::vector<std::shared_ptr<AstNodeBase>> ast;

    yy::parser::symbol_type yylex(LexerWrapper& lexer)
    {
        return lexer.yylex();
    }

    void yy::parser::error(const std::string& msg)
    {
        std::cerr << "Parse error: " << msg << std::endl;
    }

}

%define api.token.prefix {PARSER_TOK_}

%token <int> INTEGER
%token <std::string> IDENTIFIER
%token
  PLUS    "+"
  //MINUS   "-"
  //STAR    "*"
  //SLASH   "/"
  //PERCENT "%"
  EQUAL   "="
  LPAREN  "("
  RPAREN  ")"
;

%token <std::string> PREDEF_FUNCTION

%type <std::shared_ptr<AstNodeBase>> statement
%type <std::shared_ptr<ExprNode>> expr
%type <std::shared_ptr<VarDeclaration>> varDecl
%type <std::vector<std::shared_ptr<ExprNode>>> exprList
%type <std::shared_ptr<AstNodeBase>> func

%right EQUAL
%left PLUS

%start program

%%
program:
    /* empty */
    | program statement
    ;

statement:
    varDecl
    {
        ast.push_back($1);
    }
    | func
    {
        ast.push_back($1);
    }
    | expr
    {
        ast.push_back($1);
    }
    ;

func:
    PREDEF_FUNCTION "(" exprList ")"
    {
        if ($1 == "print")
            $$ = std::make_shared<PrintFunctionNode>($3);
    }
    ;

exprList:
    expr
    {
        $$ = std::vector<std::shared_ptr<ExprNode>>{ $1 };
    }
    | exprList expr
    {
        $1.push_back($2);
        $$ = $1;
    }
    ;

varDecl:
    IDENTIFIER IDENTIFIER
    {
        if ($1 == "int32")
            $$ = std::make_shared<VarDeclaration>(ExprType(ExprTypeBase::INT), $2);
        else
            $$ = std::make_shared<VarDeclaration>(ExprType($1), $2);
    }
    ;

expr:
    INTEGER
    {
        $$ = std::make_shared<Integer>($1);
    }
    | IDENTIFIER
    {
        $$ = std::make_shared<VarAccessorNode>($1);
    }
    | expr EQUAL expr %prec EQUAL
    {
        $$ = std::make_shared<AssignationNode>($1, $3);
    }
    | expr PLUS expr %prec PLUS
    {
        $$ = std::make_shared<BinaryOperatorNode>(BinaryOperation::ADD, $1, $3);
    }
    ;

