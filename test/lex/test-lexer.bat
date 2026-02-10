@echo off

mkdir build
type nul > test-to-lex-res.txt

cd ../bin

lexer.exe < ../test/test-to-lex.txt > ../test/build/test-to-lex-res.txt
