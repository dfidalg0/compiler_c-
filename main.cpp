#include<iostream>
#include "./lexer/lib/lexer.h"

int main(int argc, char **argv) {
    if(argc != 2) {
        std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
        exit(1);
    }

    auto lexer = Lexer(argv[1]);

    auto token = lexer.lex();

    while (token.type() != TokenType::END_OF_FILE) {
        token.print();
        token = lexer.lex();
    }

    return 0;
}