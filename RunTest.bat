@echo off
echo ==========================================
echo [1/3] Translating .v to C++...
echo ==========================================
bin\compiler.exe Test\test.v Test\output.cpp
if %errorlevel% neq 0 (
    echo.
    echo [!] Translation failed. Check your .v syntax!
    exit /b %errorlevel%
)

echo.
echo ==========================================
echo [2/3] Compiling C++ to Executable...
echo ==========================================
clang++ Test\output.cpp -o Test\program.exe
if %errorlevel% neq 0 (
    echo.
    echo [!] C++ Compilation failed. Check your generated AST code!
    exit /b %errorlevel%
)

echo.
echo ==========================================
echo [3/3] Running Program:
echo ==========================================
Test\program.exe

echo.
echo ==========================================
echo [+] Test Complete!