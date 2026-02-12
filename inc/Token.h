#pragma once

enum Token
{
    TOK_EOF = 0,

    TOK_INTEGER = 258,
    TOK_IDENT,

    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,
    TOK_SLASH,
    TOK_PERCENT,
    TOK_EQUAL,

    TOK_LPAREN,
    TOK_RPAREN,

    TOK_PREDEF_FUNCTION
};