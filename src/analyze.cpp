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

Code * _analyze(TreeNode * tree) {
    if (!current_scope) {
        current_scope = root;
    }

    switch (tree->nodekind) {
        case Expression: {
            switch (tree->kind.expression) {
                case Operation: {
                    auto code_l = _analyze(tree->child[0]);
                    auto code_r = _analyze(tree->child[1]);

                    if (code_l->type == Void || code_r->type == Void) {
                        auto line = tree->line;

                        std::cerr << "[Line " << line << "] Cannot perform operation on void type" << std::endl;
                        return new Code(Void);
                    }

                    return new Code(Integer);
                }
                case Constant: {
                    return new Code(Integer);
                }
                case Identifier: {
                    auto name = tree->attr.name;

                    auto var = find_variable(name);

                    if (!var) {
                        auto line = tree->line;

                        std::cerr << "[Line " << line << "] Variable " << name << " not declared" << std::endl;

                        return new Code(Void);
                    }

                    return new Code(var->type, var->size >= 0);
                }
                case Array: {
                    auto name = tree->child[0]->attr.name;

                    auto var = find_variable(name);

                    if (!var) {
                        auto line = tree->line;

                        std::cerr << "[Line " << line << "] Variable " << name << " not declared" << std::endl;

                        return new Code(Void);
                    }

                    return new Code(var->type);
                }
                case Function: {
                    auto name = tree->attr.name;

                    auto type = tree->type;

                    auto & functions = root->functions;

                    avance_scope();

                    nearest_fn_scope = current_scope;

                    _analyze(tree->child[0]);

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

                    _analyze(tree->child[1]);

                    back_scope();

                    nearest_fn_scope = nullptr;

                    return new Code(Void);
                }
                case FunctionCall: {
                    auto name = tree->attr.name;

                    auto & functions = root->functions;

                    if (functions.find(name) == functions.end()) {
                        auto line = tree->line;

                        std::cerr << "[Line " << line << "] Function " << name << " not declared" << std::endl;

                        return new Code(Void);
                    }

                    auto fn = functions[name];

                    auto args = _analyze(tree->child[0]);

                    auto args_passed = 0;
                    auto t = args;

                    while (t->next) {
                        t = t->next;
                        args_passed++;
                    }

                    auto it = 0;

                    while (t->next && fn->params.size() < it) {
                        auto param = fn->params[it];
                        auto p = t->next;

                        it++;

                        if (param->type != p->type || p->is_array && param->size < 0) {
                            auto line = tree->line;

                            std::cerr << "[Line " << line << "] Type mismatch in function call" << std::endl;

                            return new Code(Void);
                        }

                        t = p;
                    }

                    if (fn->params.size() != args_passed) {
                        auto line = tree->line;

                        std::cerr << "[Line " << line << "] Function " << name << " called with wrong number of arguments" << std::endl;

                        return new Code(Void);
                    }

                    return new Code(fn->return_type);
                }
                case ParamsList: {
                    auto stmt = tree->sibling;

                    auto code_root = new Code(Void);
                    Code * current = code_root;

                    while (stmt) {
                        current->next = _analyze(stmt);
                        stmt = stmt->sibling;
                        current = current->next;
                    }

                    return code_root;
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

                    return new Code(Void);
                }
                case Assign: {
                    auto var_node = tree->child[0];

                    std::string name = var_node->kind.expression == Array
                        ? var_node->child[0]->attr.name
                        : var_node->attr.name;

                    auto code_l = _analyze(tree->child[0]);
                    auto code_r = _analyze(tree->child[1]);

                    if (code_l->type == Void || code_r->type == Void || code_l->is_array != code_r->is_array) {
                        auto line = tree->line;

                        std::cerr << "[Line " << line << "] Type mismatch in assignment" << std::endl;

                        return new Code(Void);
                    }

                    return new Code(Void);
                }
                case If: {
                    avance_scope();

                    auto condition_code = _analyze(tree->child[0]);

                    if (condition_code->type != Integer) {
                        auto line = tree->line;

                        std::cerr << "[Line " << line << "] Type mismatch in if condition" << std::endl;

                        return new Code(Void);
                    }

                    _analyze(tree->child[1]);

                    back_scope();

                    if (tree->child[2]) {
                        avance_scope();
                        _analyze(tree->child[2]);
                        back_scope();
                    }

                    return new Code(Void);
                }
                case While: {
                    avance_scope();

                    _analyze(tree->child[1]);

                    back_scope();

                    return new Code(Void);
                }
                case Return: {
                    if (!nearest_fn_scope) {
                        auto line = tree->line;

                        std::cerr << "[Line " << line << "] Error: return outside function" << std::endl;
                    }
                    else {
                        _analyze(tree->child[0]);
                    }

                    return new Code(Void);
                }
                case StatementList: {
                    auto stmt = tree->sibling;

                    while (stmt) {
                        _analyze(stmt);
                        stmt = stmt->sibling;
                    }

                    return new Code(Void);
                }
            }
            break;
        }
    }

    return nullptr;
}

Code * analyze(TreeNode * tree) {
    if (!tree) {
        std::cerr << "analyze: tree is null" << std::endl;
        return nullptr;
    }

    auto code = _analyze(tree);
    auto it = root->functions.find("main");
    if(it == root->functions.end()){
        std::cerr << "Error: Function main not declared" << std::endl;
    }

    return code;
}
