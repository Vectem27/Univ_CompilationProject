@echo off
setlocal

if not exist build (
    mkdir build
)

> build\test-code-res.c echo.

cd ../bin

compiler.exe < ../test/test-code.txt > ../test/build\test-code-res.c

endlocal
