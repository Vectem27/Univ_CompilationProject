#include <iostream>
#include <memory>
#include <vector>

#include "FlexLexer.h"
#include "parser.tab.hh"   // header généré par Bison
#include "Ast.h"

// AST global défini dans parser.y
extern std::vector<std::unique_ptr<Stmt>> ast;

static yyFlexLexer lexer;
int yylex() { return lexer.yylex(); }

void yyerror(const char* s) {
    std::cerr << "Erreur: " << s << std::endl;
}

int main() {
    std::cout << "Entrez votre programme (Ctrl+D pour finir) :" << std::endl;

    // Appel du parser Bison
    int parseResult = yyparse();

    if (parseResult == 0) {
        std::cout << "Analyse terminée avec succès !" << std::endl;

        // Affichage de l'AST
        for (const auto& stmt : ast) {
            stmt->print();
        }
    } else {
        std::cerr << "Erreur de parsing !" << std::endl;
    }

    return parseResult;
}
