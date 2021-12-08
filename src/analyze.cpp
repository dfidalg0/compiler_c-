#include <analyze.hpp>

Scope * root = new Scope(nullptr);

Scope * current_scope = nullptr;

Scope * nearest_fn_scope = nullptr;

void avance_scope() {
    current_scope = new Scope(current_scope);
}

void back_scope() {
    auto parent = current_scope->parent;
    delete current_scope;
    current_scope = parent;
}

Variable * find_variable(const std::string & name) {
    auto scope = current_scope;
    while (scope != nullptr) {
        auto it = scope->variables.find(name);
        if (it != scope->variables.end()) {
            return it->second;
        }
        scope = scope->parent;
    }
    return nullptr;
}

void analyzeHelper(TreeNode * tree) {
    if (!current_scope) {
        current_scope = root;
    }

    switch (tree->nodekind) {
        case Expression: {
            switch (tree->kind.expression) {
                case Operation: {
                    analyzeHelper(tree->child[0]);
                    analyzeHelper(tree->child[1]);
                    break;
                }
                case Constant: {
                    break;
                }
                case Identifier: {
                    auto name = tree->attr.name;

                    auto var = find_variable(name);

                    if (!var) {
                        auto line = tree->line;

                        std::cerr << "[Line " << line << "] Variable " << name << " not declared" << std::endl;
                    }

                    break;
                }
                case Array: {
                    auto name = tree->child[0]->attr.name;

                    auto var = find_variable(name);

                    if (!var) {
                        auto line = tree->line;

                        std::cerr << "[Line " << line << "] Variable " << name << " not declared" << std::endl;
                    }

                    break;
                }
                case Function: {
                    auto name = tree->attr.name;

                    auto type = tree->type;

                    auto & functions = root->functions;

                    avance_scope();

                    nearest_fn_scope = current_scope;

                    analyzeHelper(tree->child[0]);

                    std::vector<Variable *> variables;

                    for (auto it : current_scope->variables) {
                        variables.push_back(it.second);
                    }

                    if (functions.find(name) != functions.end()) {
                        auto line = tree->line;

                        std::cerr << "[Line " << line << "] Function " << name << " already declared" << std::endl;
                    } else {
                        functions[name] = new Fn(name, variables, type);
                    }

                    analyzeHelper(tree->child[1]);

                    back_scope();

                    nearest_fn_scope = nullptr;

                    break;
                }
                case FunctionCall: {
                    auto name = tree->attr.name;

                    auto & functions = root->functions;

                    if (functions.find(name) == functions.end()) {
                        auto line = tree->line;

                        std::cerr << "[Line " << line << "] Function " << name << " not declared" << std::endl;
                    }
                    else {
                        auto fn = functions[name];

                        auto args_passed = 0;
                        auto t = tree->child[0];
                        while (t->sibling) {
                            args_passed++;
                            t = t->sibling;
                        }

                        if (fn->params.size() != args_passed) {
                            auto line = tree->line;

                            std::cerr << "[Line " << line << "] Function " << name << " called with wrong number of arguments" << std::endl;
                        }
                    }

                    analyzeHelper(tree->child[0]);

                    break;
                }
                case ParamsList: {
                    auto stmt = tree->sibling;

                    while (stmt) {
                        analyzeHelper(stmt);
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

                    if(tree->type == Void){
                        auto line = tree->line;

                        std::cerr << "[Line " << line << "] Error: Invalid declaration, void can only be used for functions" << std::endl;
                    }

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
                        auto line = tree->line;

                        std::cerr << "[Line " << line << "] Error: variable " << var->name << " already declared" << std::endl;
                    }
                    else {
                        variables[var->name] = var;
                    }

                    break;
                }
                case Assign: {
                    auto var_node = tree->child[0];

                    std::string name = var_node->kind.expression == Array
                        ? var_node->child[0]->attr.name
                        : var_node->attr.name;

                    auto var = find_variable(name);

                    if (!var) {
                        auto line = tree->line;

                        std::cerr << "[Line " << line << "] Error: variable " << name << " not declared" << std::endl;
                    }

                    analyzeHelper(tree->child[1]);

                    break;
                }
                case If: {
                    avance_scope();

                    analyzeHelper(tree->child[1]);

                    back_scope();

                    if (tree->child[2]) {
                        avance_scope();
                        analyzeHelper(tree->child[2]);
                        back_scope();
                    }

                    break;
                }
                case While: {
                    avance_scope();

                    analyzeHelper(tree->child[1]);

                    back_scope();

                    break;
                }
                case Return: {
                    if (!nearest_fn_scope) {
                        auto line = tree->line;

                        std::cerr << "[Line " << line << "] Error: return outside function" << std::endl;
                    }
                    else {
                        analyzeHelper(tree->child[0]);
                    }

                    break;
                }
                case StatementList: {
                    auto stmt = tree->sibling;

                    while (stmt) {
                        analyzeHelper(stmt);
                        stmt = stmt->sibling;
                    }

                    break;
                }
            }
            break;
        }
    }
}

void analyze(TreeNode * tree) {
    if (!tree) {
        std::cout << "analyze: tree is null" << std::endl;
        return;
    }

   analyzeHelper(tree);
   auto it = root->functions.find("main");
   if(it == root->functions.end()){
       std::cout << "Error: Function main not declared" << std::endl;
   }
}