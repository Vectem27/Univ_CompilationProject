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
%token <double> FLOAT
%token <std::string> STRING
%token <std::string> IDENTIFIER
%token
  PLUS    "+"
  MINUS   "-"
  STAR    "*"
  SLASH   "/"
  PERCENT "%"
  EQUAL   "="
  LPAREN  "("
  RPAREN  ")"
    LBRACE  "{"
    RBRACE  "}"
  SEMICOLON ";"
  EQEQ    "=="
  NEQ     "!="
  LT      "<"
  LTE     "<="
  GT      ">"
  GTE     ">="
;

%token TRUE_LIT "true"
%token FALSE_LIT "false"
%token IF_KW "if"
%token ELSE_KW "else"
%token WHILE_KW "while"
%token DO_KW "do"

%token <std::string> PREDEF_FUNCTION

%type <std::shared_ptr<AstNodeBase>> statement
%type <std::shared_ptr<AstNodeBase>> block
%type <std::shared_ptr<ExprNode>> expr
%type <std::shared_ptr<VarDeclaration>> varDecl
%type <std::vector<std::shared_ptr<ExprNode>>> exprList
%type <std::vector<std::shared_ptr<AstNodeBase>>> statementList
%type <std::shared_ptr<AstNodeBase>> func

%nonassoc NO_ELSE
%nonassoc "else"
%right EQUAL
%left EQEQ NEQ
%left LT LTE GT GTE
%left PLUS MINUS
%left STAR SLASH PERCENT



%start program

%%
program:
    /* empty */
    | program statement
    {
        ast.push_back($2);
    }
    ;

statement:
    varDecl ";"
    {
        $$ = $1;
    }
    | func ";"
    {
        $$ = $1;
    }
    | expr ";"
    {
        $$ = $1;
    }
    | block
    {
        $$ = $1;
    }
    | "if" "(" expr ")" statement %prec NO_ELSE
    {
        $$ = std::make_shared<IfNode>($3, $5, nullptr);
    }
    | "if" "(" expr ")" statement "else" statement
    {
        $$ = std::make_shared<IfNode>($3, $5, $7);
    }
    | "while" "(" expr ")" statement
    {
        $$ = std::make_shared<WhileNode>($3, $5);
    }
    | "do" statement "while" "(" expr ")" ";"
    {
        $$ = std::make_shared<DoWhileNode>($5, $2);
    }
    ;

statementList:
    /* empty */
    {
        $$ = std::vector<std::shared_ptr<AstNodeBase>>{};
    }
    | statementList statement
    {
        $1.push_back($2);
        $$ = $1;
    }
    ;

block:
    "{" statementList "}"
    {
        $$ = std::make_shared<ScopeBlockNode>($2);
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
        if ($1 == "int")
            $$ = std::make_shared<VarDeclaration>(ExprType(ExprTypeBase::INT), $2);
        else if ($1 == "float")
            $$ = std::make_shared<VarDeclaration>(ExprType(ExprTypeBase::FLOAT), $2);
        else if ($1 == "string")
            $$ = std::make_shared<VarDeclaration>(ExprType(ExprTypeBase::STRING), $2);
        else if ($1 == "bool")
            $$ = std::make_shared<VarDeclaration>(ExprType(ExprTypeBase::BOOL), $2);
        else
            $$ = std::make_shared<VarDeclaration>(ExprType($1), $2);
    }
    ;

expr:
    INTEGER
    {
        $$ = std::make_shared<Integer>($1);
    }
    | FLOAT
    {
        $$ = std::make_shared<Float>($1);
    }
    | STRING
    {
        $$ = std::make_shared<String>($1);
    }
    | IDENTIFIER
    {
        $$ = std::make_shared<VarAccessorNode>($1);
    }
    | "(" expr ")"
    { 
        $$ = $2;
    }
    | expr "=" expr %prec "="
    {
        $$ = std::make_shared<AssignationNode>($1, $3);
    }
    | expr "+" expr %prec "+"
    {
        $$ = std::make_shared<BinaryOperatorNode>(BinaryOperation::ADD, $1, $3);
    }
    | expr "-" expr %prec "-"
    {
        $$ = std::make_shared<BinaryOperatorNode>(BinaryOperation::SUBSTRACT, $1, $3);
    }
    | expr "*" expr %prec "*"
    {
        $$ = std::make_shared<BinaryOperatorNode>(BinaryOperation::MULTIPLY, $1, $3);
    }
    | expr "/" expr %prec "/"
    {
        $$ = std::make_shared<BinaryOperatorNode>(BinaryOperation::DIVIDE, $1, $3);
    }
    | expr "%" expr %prec "%"
    {
        $$ = std::make_shared<BinaryOperatorNode>(BinaryOperation::MODULO, $1, $3);
    }
    | TRUE_LIT
    {
        $$ = std::make_shared<Bool>(true);
    }
    | FALSE_LIT
    {
        $$ = std::make_shared<Bool>(false);
    }
    | expr "==" expr
    {
        $$ = std::make_shared<ComparisonNode>(ComparisonOperation::EQ, $1, $3);
    }
    | expr "!=" expr
    {
        $$ = std::make_shared<ComparisonNode>(ComparisonOperation::NEQ, $1, $3);
    }
    | expr "<" expr
    {
        $$ = std::make_shared<ComparisonNode>(ComparisonOperation::LT, $1, $3);
    }
    | expr "<=" expr
    {
        $$ = std::make_shared<ComparisonNode>(ComparisonOperation::LTE, $1, $3);
    }
    | expr ">" expr
    {
        $$ = std::make_shared<ComparisonNode>(ComparisonOperation::GT, $1, $3);
    }
    | expr ">=" expr
    {
        $$ = std::make_shared<ComparisonNode>(ComparisonOperation::GTE, $1, $3);
    }

    ;

