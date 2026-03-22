#include "Token.h"
#include "parser.tab.hh"
#include "Ast.h"
#include "LexerWrapper.h"

#include <cstdlib>
#include <fstream>


std::vector<std::shared_ptr<AstNodeBase>> ast;

class TestValidator : public INodeValidator
{
public:
    virtual void Send(ENodeValidationMessageType messageType, std::string message) override
    {
        if (messageType == ENodeValidationMessageType::Critical)
            printf("Critical: %s\n", message.c_str());
        else if (messageType == ENodeValidationMessageType::Error)
            printf("Error: %s\n", message.c_str());
        else if (messageType == ENodeValidationMessageType::Warning) 
            printf("Warning: %s\n", message.c_str());
        else if (messageType == ENodeValidationMessageType::Info)
            printf("Info: %s\n", message.c_str());
    }
};

int main(int argc, char* argv[]) 
{
    if (argc != 3)
    {
        std::cerr << "2 arguments is required";
        return EXIT_FAILURE;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cerr << "Failed to open : " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    std::ofstream outputFile(argv[2]);
    if (!outputFile) {
        std::cerr << "Failed to create " << argv[2] << std::endl;
        return EXIT_FAILURE;
    }

    LexerWrapper lexer(&inputFile);

    lexer.AddToken(TOK_INTEGER, [](const std::string& text){
        return yy::parser::make_INTEGER(std::stoi(text));
    });

    lexer.AddToken(TOK_FLOAT, [](const std::string& text){
        return yy::parser::make_FLOAT(std::stod(text));
    });

    lexer.AddToken(TOK_STRING, [](const std::string& text){
        const std::size_t firstQuote = text.find('"');
        const std::size_t lastQuote = text.rfind('"');

        if (firstQuote != std::string::npos && lastQuote != std::string::npos && lastQuote > firstQuote)
            return yy::parser::make_STRING(text.substr(firstQuote + 1, lastQuote - firstQuote - 1));

        return yy::parser::make_STRING(text);
    });

    lexer.AddToken(TOK_IDENT, [](const std::string& text){
        return yy::parser::make_IDENTIFIER(text);
    });

    lexer.AddToken(TOK_PLUS, [](const std::string& text){
        return yy::parser::make_PLUS();
    });

    lexer.AddToken(TOK_MINUS, [](const std::string& text){
        return yy::parser::make_MINUS();
    });

    lexer.AddToken(TOK_STAR, [](const std::string& text){
        return yy::parser::make_STAR();
    });

    lexer.AddToken(TOK_SLASH, [](const std::string& text){
        return yy::parser::make_SLASH();
    });

    lexer.AddToken(TOK_PERCENT, [](const std::string& text){
        return yy::parser::make_PERCENT();
    });

    lexer.AddToken(TOK_PREDEF_FUNCTION, [](const std::string& text){
        return yy::parser::make_PREDEF_FUNCTION(text);
    });

    lexer.AddToken(TOK_LPAREN, [](const std::string& text){
        return yy::parser::make_LPAREN();
    });

    lexer.AddToken(TOK_RPAREN, [](const std::string& text){
        return yy::parser::make_RPAREN();
    });

    lexer.AddToken(TOK_LBRACE, [](const std::string& text){
        return yy::parser::make_LBRACE();
    });

    lexer.AddToken(TOK_RBRACE, [](const std::string& text){
        return yy::parser::make_RBRACE();
    });

    lexer.AddToken(TOK_EQUAL, [](const std::string& text){
        return yy::parser::make_EQUAL();
    });

    lexer.AddToken(TOK_EQEQ, [](const std::string& text){
        return yy::parser::make_EQEQ();
    });

    lexer.AddToken(TOK_NEQ, [](const std::string& text){
        return yy::parser::make_NEQ();
    });

    lexer.AddToken(TOK_LT, [](const std::string& text){
        return yy::parser::make_LT();
    });

    lexer.AddToken(TOK_LTE, [](const std::string& text){
        return yy::parser::make_LTE();
    });

    lexer.AddToken(TOK_GT, [](const std::string& text){
        return yy::parser::make_GT();
    });

    lexer.AddToken(TOK_GTE, [](const std::string& text){
        return yy::parser::make_GTE();
    });

    lexer.AddToken(TOK_TRUE, [](const std::string& text){
        return yy::parser::make_TRUE_LIT();
    });

    lexer.AddToken(TOK_FALSE, [](const std::string& text){
        return yy::parser::make_FALSE_LIT();
    });

    lexer.AddToken(TOK_IF, [](const std::string& text){
        return yy::parser::make_IF_KW();
    });

    lexer.AddToken(TOK_ELSE, [](const std::string& text){
        return yy::parser::make_ELSE_KW();
    });

    lexer.AddToken(TOK_WHILE, [](const std::string& text){
        return yy::parser::make_WHILE_KW();
    });

    lexer.AddToken(TOK_DO, [](const std::string& text){
        return yy::parser::make_DO_KW();
    });

    lexer.AddToken(TOK_EOF, [](const std::string& text){
        return yy::parser::make_YYEOF();
    });
    lexer.AddToken(TOK_SEMICOLON, [](const std::string& text){
        return yy::parser::make_SEMICOLON();
    });



    std::cout << "Analyze ..." << std::endl;

    yy::parser parserInstance(lexer);
    int parseResult = parserInstance.parse();

    if (parseResult != 0) 
    {
        std::cerr << "Parsing error" << std::endl;
        return EXIT_FAILURE;
    }
        
    TestValidator validator = TestValidator();
    for (auto& node : ast)
    {
        if (node->Validate(validator))
            continue;
        std::cerr << "Nodes validation failed" << std::endl;
        return EXIT_SUCCESS;
    }

    outputFile << "#include <string>" << std::endl;
    outputFile << "#include <iostream>" << std::endl;
    outputFile << std::endl;
    outputFile << "int main()" << std::endl << "{" << std::endl;

    for (auto& node : ast)
    {
        outputFile << "    ";
        node->GenerateCode(outputFile);
        outputFile << ";" << std::endl;
    }

    outputFile << "    return EXIT_SUCCESS;" << std::endl;
    outputFile << "}" << std::endl;


    std::cout << "Nodes : " << ast.size() << std::endl;

    std::cout << "Success" << std::endl;

    return EXIT_SUCCESS;
}
