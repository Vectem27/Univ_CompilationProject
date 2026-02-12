#include "parser.tab.hh"
#include "Ast.h"
#include "LexerWrapper.h"

#include <fstream>



std::vector<std::shared_ptr<AstNodeBase>> ast;

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

int main(int argc, char* argv[]) 
{
    if (argc != 2)
    {
        std::cerr << "1 arguments is required";
        return EXIT_FAILURE;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cerr << "Impossible d'ouvrir le fichier " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    LexerWrapper lexer(&inputFile);

    lexer.AddToken(TOK_INTEGER, [](const std::string text){
        return yy::parser::make_INTEGER(std::stoi(text));
    });

    lexer.AddToken(TOK_PLUS, [](const std::string text){
        return yy::parser::make_PLUS();
    });

    lexer.AddToken(TOK_EOF, [](const std::string text){
        return yy::parser::make_YYEOF();
    });


    std::cout << "Analyze ..." << std::endl;

    yy::parser parserInstance = yy::parser(lexer);
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
