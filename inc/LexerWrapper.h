#pragma once

#include "FlexLexer.h"
#include "Token.h"
#include "parser.tab.hh"

#include <functional>
#include <stdexcept>
#include <istream>
#include <cstdio>
#include <unordered_map>

// TODO: Enter tokens dynamically

class LexerWrapper 
{
    yyFlexLexer lexer;
public:
    LexerWrapper(std::istream* in) : lexer(in) {}

    void AddToken(Token token, std::function<yy::parser::symbol_type(const std::string&)> resolver) {tokens[token] = resolver; }

    yy::parser::symbol_type yylex() 
    {
        Token tok = (Token)lexer.yylex();

        try
        {
            return tokens[tok](lexer.YYText());
        }
        catch(...)
        {
            throw std::runtime_error("Token inconnu");
        }
    }
private:
    std::unordered_map<Token, std::function<yy::parser::symbol_type(const std::string&)>> tokens;
};