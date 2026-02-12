@echo off

set GREEN = [32m
set RESET_COLOR = [0m

echo %GREEN%[+] Initalize...%RESET_COLOR%

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

echo %GREEN%[+] Build lexer and parser...%RESET_COLOR%

bison -d --language=c++ -v -o build/lexer_parser/parser.tab.cc parser.y
clang++ -std=c++20 -Iinc -w -c build/lexer_parser/parser.tab.cc -o build/lexer_parser/obj/parser.tab.o

flex -obuild/lexer_parser/lex.yy.cc lexer.l
clang++ -std=c++20 -Iinc -w -c build/lexer_parser/lex.yy.cc -o build/lexer_parser/obj/lex.yy.o

echo %GREEN%[+] Build Project...%RESET_COLOR%

clang++ -std=c++20 src/main.cpp src/Ast.cpp -Ibuild/lexer_parser -Iinc build/lexer_parser/obj/parser.tab.o build/lexer_parser/obj/lex.yy.o -o bin/compiler.exe
