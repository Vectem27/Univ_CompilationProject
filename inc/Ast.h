// ast.h
#pragma once
#include <string>
#include <iostream>

struct Stmt 
{
    virtual ~Stmt() = default;
    virtual void print() const = 0;
};

struct PrintStmt : Stmt 
{
    int value;
    PrintStmt(int v) : value(v) {}
    void print() const override { std::cout << value << "\n"; }
};

struct LetStmt : Stmt 
{
    std::string name;
    int value;
    LetStmt(const std::string &n, int v) : name(n), value(v) {}
    void print() const override { std::cout << "Variable " << name << " = " << value << "\n"; }
};
