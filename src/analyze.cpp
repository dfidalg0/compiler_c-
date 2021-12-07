#include <analyze.hpp>

Scope * root = new Scope(nullptr);

Scope * current_scope = nullptr;

void avance_scope() {
    current_scope = new Scope(current_scope);
}

void back_scope() {
    auto parent = current_scope->parent;
    delete current_scope;
    current_scope = parent;
}

void analyze(TreeNode * tree) {
    if (!tree) {
        return;
    }

    if (!current_scope) {
        current_scope = root;
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
                    avance_scope();

                    analyze(tree->child[0]);

                    analyze(tree->child[1]);

                    back_scope();

                    break;
                }
                case FunctionCall: {
                    break;
                }
                case ParamsList: {
                    auto stmt = tree->sibling;

                    while (stmt) {
                        analyze(stmt);
                        stmt = stmt->sibling;
                    }

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

                    auto & variables = current_scope->variables;

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

                    bool found = false;

                    auto it = current_scope;

                    while (!found && it) {
                        auto & variables = it->variables;

                        // Buscamos a variável no escopo atual
                        if (variables.find(name) != variables.end()) {
                            found = true;
                        }
                        // Caso contrário, buscamos na hierarquia de escopos
                        else {
                            it = it->parent;
                        }
                    }

                    if (!found) {
                        std::cerr << "Error: variable " << name << " not declared" << std::endl;
                    }

                    break;
                }
                case If: {
                    avance_scope();

                    analyze(tree->child[1]);

                    back_scope();

                    if (tree->child[2]) {
                        avance_scope();
                        analyze(tree->child[2]);
                        back_scope();
                    }

                    break;
                }
                case While: {
                    avance_scope();

                    analyze(tree->child[1]);

                    back_scope();

                    break;
                }
                case Return: {
                    break;
                }
                case StatementList: {
                    auto stmt = tree->sibling;

                    while (stmt) {
                        analyze(stmt);
                        stmt = stmt->sibling;
                    }

                    break;
                }
            }
            break;
        }
    }
}
