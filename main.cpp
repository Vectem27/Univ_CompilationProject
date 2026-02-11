#include <iostream>

extern "C" {
    int yyparse();
}

int main() {
    std::cout << "Début du parsing\n";
    return yyparse();
}
