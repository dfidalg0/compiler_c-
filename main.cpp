#include <iostream>
#include <parser.yy.hpp>
#include <analyze.hpp>

extern FILE * yyin;

int main(int argc, char **argv) {
    if (argc == 2) {
        yyin = fopen(argv[1], "r");
    }
    else if (argc > 2) {
        std::cout << "Usage: " << argv[0] << " [filename]" << std::endl;
        return 1;
    }

    auto tree = getTree();
    printTree(tree);

    analyze(tree);

    return 0;
}
