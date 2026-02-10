@echo off

bison -d --language=c++ parser.y
flex++ lexer.l
ren lex.yy.cc lexer.cpp

if not exist bin (
    mkdir bin
)

clang -std=c++20 -isystem -I"./inc" -o bin/compiler.exe parser.tab.cc lexer.cpp main.cpp

REM