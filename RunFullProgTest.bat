@echo off
echo ==========================================
echo [1/3] Translating .v to C++...
echo ==========================================
bin\compiler.exe Test\test_prog.v Test\test_prog.cpp
if %errorlevel% neq 0 (
    echo.
    echo [!] Translation failed. Check your .v syntax!
    exit /b %errorlevel%
)

echo.
echo ==========================================
echo [2/3] Compiling C++ to Executable...
echo ==========================================
clang++ Test\test_prog.cpp -o Test\test_prog.exe
if %errorlevel% neq 0 (
    echo.
    echo [!] C++ Compilation failed. Check your generated AST code!
    exit /b %errorlevel%
)
echo Success!

echo.
echo ==========================================
echo [3/3] Running Program:
echo ==========================================
Test\test_prog.exe

echo.
echo ==========================================
echo [+] Test Complete!