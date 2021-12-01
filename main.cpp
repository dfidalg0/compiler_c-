#include <iostream>
#include "./lexer/lib/lexer.h"
#include "./parser/lib/parse.h"
#include "./parser/lib/util.h"

#define TYPE 1

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
        exit(1);
    }

    if (TYPE == 1)
    {
        auto lexer = Lexer(argv[1]);

        auto token = lexer.lex();

        while (token.type() != yytokentype::END_OF_FILE)
        {
            token.print();
            token = lexer.lex();
        }
    }
    
    return 0;
}