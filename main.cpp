#include <iostream>
#include <parser.yy.hpp>
// #include <lexer.hpp>
// #include <parse.hpp>
// #include <util.hpp>

// #define TYPE 1

extern FILE * yyin;
extern TreeNode* getTree();

int main(int argc, char **argv) {
    if (argc == 2) {
        yyin = fopen(argv[1], "r");
    }
    else if (argc > 2) {
        std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    auto tree = getTree();
    printTree(tree);

    //auto type = yylex();

    //while (type != yytokentype::YYEOF) {
    //    yylval.token->print();
    //    type = yylex();
    //}

    return 0;
}
