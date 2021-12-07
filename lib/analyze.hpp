#include <tree.hpp>
#include <string>
#include <vector>
#include <map>
#include <iostream>

struct Variable {
    Variable(std::string name, ExpressionType type) : name(name), type(type), size(-1) {}
    Variable(std::string name, ExpressionType type, int size) : name(name), type(type), size(size) {}
    std::string name;
    ExpressionType type;
    int size;
};

struct Fn {
    Fn(std::string name, ExpressionType type) : name(name), type(type) {}
    std::string name;
    ExpressionType type;
    std::vector<Variable> args;
};

struct Scope {
    Scope(Scope *parent) : parent(parent) {}
    Scope *parent;
    std::map<std::string, Variable *> variables;
    std::map<std::string, Fn *> functions;
};

void analyze(TreeNode *tree);
