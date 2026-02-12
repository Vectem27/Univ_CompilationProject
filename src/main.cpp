#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

//#include "FlexLexer.h"   // Flex given class
#include "parser.tab.hh" // Bison generated header
#include "Ast.h"

std::vector<std::shared_ptr<AstNodeBase>> ast;
//static yyFlexLexer lexer;
//int yylex() { return lexer.yylex(); }

yy::parser::symbol_type yylex()
{
    static int count = 0;
    switch (int stage = count++)
    {
    case 0:
        return yy::parser::make_INTEGER (5);
    case 1:
        return yy::parser::make_PLUS ();
    case 2:
        return yy::parser::make_INTEGER (3);
    default:
        return yy::parser::make_YYEOF ();
    }
}

void yyerror(const char* s) {
    std::cerr << "Erreur: " << s << std::endl;
}

class TestValidator : public INodeValidator
{
public:
    virtual void Send(ENodeValidationMessageType messageType, std::string message) override
    {
        if (messageType == ENodeValidationMessageType::Critical)
            printf("Critical: %s", message.c_str());
        else if (messageType == ENodeValidationMessageType::Error)
            printf("Error: %s", message.c_str());
        else if (messageType == ENodeValidationMessageType::Warning)
            printf("Warning: %s", message.c_str());

        printf("Info: %s", message.c_str());
    }
};

int main() {
    std::cout << "Analyze ..." << std::endl;

    yy::parser parserInstance = yy::parser();
    int parseResult = parserInstance.parse();

    if (parseResult != 0) 
    {
        std::cerr << "Parsing error" << std::endl;
        return EXIT_FAILURE;
    }
        
    TestValidator validator = TestValidator();
    for (auto& node : ast)
        node->Validate(validator);

    std::cout << "Nodes : " << ast.size() << std::endl;

    std::cout << "Success" << std::endl;

    return EXIT_SUCCESS;
}
