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
%token
  PLUS    "+"
;

%type <std::shared_ptr<ExprNode>> expr

%left "+";

%%

expr:
      expr PLUS expr
      {
          std::cout << "Plus" << std::endl;
          $$ = std::make_shared<BinaryOperatorNode>(BinaryOperation::ADD, $1, $3);
          ast.push_back(std::static_pointer_cast<ExprNode>($$));
      }
    | INTEGER
      {
          std::cout << "Int: " << $1 << std::endl;
          $$ = std::make_shared<Integer>($1);
          ast.push_back(std::static_pointer_cast<ExprNode>($$));
      }
    ;

