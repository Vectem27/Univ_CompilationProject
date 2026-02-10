#include <FlexLexer.h>
#include "parser.tab.hh"

int main() {
    yy::parser parser;
    parser.parse();
    return 0;
}
