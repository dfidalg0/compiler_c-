#include <iostream>
#include <parser.yy.hpp>
#include <analyze.hpp>
#include <globals.hpp>

extern FILE *yyin;

int line_start = 1, line_end = 1, col_start = 1, col_end = 1;
FILE *table_simbols;

int main(int argc, char **argv) {
    if (argc == 2) {
        yyin = fopen(argv[1], "r");

        if (!yyin) {
            std::cout << "File not found " << argv[1] << std::endl;
            exit(1);
        }
    }
    else if (argc > 2) {
        std::cout << "Usage: " << argv[0] << " [filename]" << std::endl;
        return 1;
    }

    auto tree = getTree();
    if(tree) {
        std::cout << "Árvore sintática:\n\n";
        printTree(tree);

        auto code = analyze(tree);

        if (code) {
            std::cout << "\n\nCódigo intermediário:\n\n";
            std::cout << code->stmt << std::endl;
        }
        else {
            std::cout << "Código intermediário não gerado devido a erros" << std::endl;
        }
    }


    printSymbolsTable();
    return 0;
}
