@echo off


if not exist bin (
    mkdir bin
)

if not exist build (
    mkdir build
)

cd build

if not exist lexer_parser (
    mkdir lexer_parser
)

cd lexer_parser
if not exist obj (
    mkdir obj
)
cd ../..

bison -d -o build/lexer_parser/parser.tab.c parser.y 
flex -obuild/lexer_parser/lex.yy.c lexer.l


clang -w -c build/lexer_parser/parser.tab.c -o build/lexer_parser/obj/parser.tab.o
clang -w -c build/lexer_parser/lex.yy.c -o build/lexer_parser/obj/lex.yy.o

clang++ -std=c++17 main.cpp build/lexer_parser/obj/parser.tab.o build/lexer_parser/obj/lex.yy.o -o bin/compiler.exe