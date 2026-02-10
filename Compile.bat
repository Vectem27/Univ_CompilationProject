@echo off

flex lexer.l
bison -d parser.y

mkdir bin
clang -std=c99 -o bin/lexer.exe lex.yy.c parser.tab.c

REM -I"C:\Program Files (x86)\GnuWin32\include"