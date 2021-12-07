#include <analyze.hpp>

Scope * root = new Scope(nullptr);

Scope * current_scope = nullptr;
Scope * prev_scope = nullptr;

void analyze(TreeNode * tree) {
    if (!tree) {
        return;
    }

    if (!current_scope) {
        current_scope = prev_scope = root;
    }

    switch (tree->nodekind) {
        case Expression: {
            switch (tree->kind.expression) {
                case Operation: {
                    break;
                }
                case Constant: {
                    break;
                }
                case Identifier: {
                    break;
                }
                case Array: {
                    break;
                }
                case Function: {
                    break;
                }
                case FunctionCall: {
                    break;
                }
                case ParamsList: {
                    break;
                }
            }
            break;
        }
        case Statement: {
            switch (tree->kind.statement) {
                case VariableDeclaration: {
                    auto child = tree->child[0];

                    Variable * var;

                    if (child->kind.expression == Array) {
                        auto size = child->attr.pos;
                        auto id = child->child[0];
                        auto type = Integer;

                        var = new Variable(id->attr.name, type, size);
                    }
                    else {
                        auto name = child->attr.name;
                        auto type = Integer;

                        var = new Variable(name, type);
                    }

                    auto variables = current_scope->variables;

                    if (variables.find(var->name) != variables.end()) {
                        std::cerr << "Error: variable " << var->name << " already declared" << std::endl;
                    }
                    else {
                        variables[var->name] = var;
                    }

                    break;
                }
                case Assign: {
                    auto var = tree->child[0];

                    std::string name = var->kind.expression == Array
                        ? var->child[0]->attr.name
                        : var->attr.name;

                    auto variables = current_scope->variables;

                    if (variables.find(name) == variables.end()) {
                        std::cerr << "Error: variable " << name << " not declared" << std::endl;
                    }

                    break;
                }
                case If: {
                    break;
                }
                case While: {
                    break;
                }
                case Return: {
                    break;
                }
                case StatementList: {
                    auto child = tree->child[0];

                    while (child) {
                        analyze(child);
                        child = child->sibling;
                    }

                    break;
                }
            }
            break;
        }
    }
}
