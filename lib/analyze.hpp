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

struct Code {
    Code(ExpressionType type) : type(type), is_array(false), next(nullptr) {}
    Code(ExpressionType type, bool is_array) : type(type), is_array(is_array), next(nullptr) {}
    ExpressionType type;
    bool is_array;
    Code * next;
};

struct Fn {
    Fn(std::string name, std::vector<Variable *> params, ExpressionType type):
        name(name), return_type(type), params(params) {}
    std::string name;
    std::vector<Variable *> params;
    ExpressionType return_type;
};

struct Scope {
    Scope(Scope *parent) : parent(parent) {}
    Scope *parent;
    std::map<std::string, Variable *> variables;
    std::map<std::string, Fn *> functions;
};

Code * analyze(TreeNode *tree);
