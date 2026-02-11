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

bison -d -v -o build/lexer_parser/parser.tab.cc parser.y
flex -obuild/lexer_parser/lex.yy.cc lexer.l

clang++ -std=c++20 -Iinc -w -c build/lexer_parser/parser.tab.cc -o build/lexer_parser/obj/parser.tab.o
clang++ -std=c++20 -Iinc -w -c build/lexer_parser/lex.yy.cc -o build/lexer_parser/obj/lex.yy.o

clang++ -std=c++20 main.cpp  -Ibuild/lexer_parser -Iinc build/lexer_parser/obj/parser.tab.o build/lexer_parser/obj/lex.yy.o -o bin/compiler.exe
