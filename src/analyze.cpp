#include <analyze.hpp>

Scope * root = new Scope(nullptr);

Scope * current_scope = nullptr;

Scope * nearest_fn_scope = nullptr;

bool err = false;

int _rn = 1;

std::string rn() {
    return std::to_string(_rn++);
}

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

                        err = true;

                        std::cerr << "[Line " << line << "] Cannot perform operation on void type" << std::endl;
                        return new Code(Void);
                    }

                    auto code = new Code(Integer);

                    auto reg = "$r" + rn();

                    auto op = getOperationType(tree->attr.operation);

                    code->stmt += code_l->stmt;
                    code->stmt += code_r->stmt;
                    code->stmt += reg + " = " + code_l->reg + " " + op + " " + code_r->reg + "\n";

                    code->reg = reg;

                    return code;
                }
                case Constant: {
                    auto code = new Code(Integer);

                    code->reg = std::to_string(tree->attr.val);

                    return code;
                }
                case Identifier: {
                    auto name = tree->attr.name;

                    auto var = find_variable(name);

                    if (!var) {
                        auto line = tree->line;

                        err = true;
                        std::cerr << "[Line " << line << "] Variable " << name << " not declared" << std::endl;

                        return new Code(Void);
                    }

                    auto code = new Code(var->type, var->size >= 0);

                    code->reg = name;

                    return code;
                }
                case Array: {
                    std::string name = tree->child[0]->attr.name;

                    auto var = find_variable(name);

                    if (!var) {
                        auto line = tree->line;
                        err = true;
                        std::cerr << "[Line " << line << "] Variable " << name << " not declared" << std::endl;

                        return new Code(Void);
                    }

                    auto code = new Code(var->type);

                    if (tree->child[1]) {
                        auto code_index = _analyze(tree->child[1]);

                        if (code_index->type != Integer) {
                            auto line = tree->line;
                            err = true;
                            std::cerr << "[Line " << line << "] Array index must be integer" << std::endl;

                            return new Code(Void);
                        }

                        auto reg = "$r" + rn();

                        code->stmt += code_index->stmt;
                        code->stmt += reg + " = " + code_index->reg + "\n";

                        code->reg = name + "[" + reg + "]";
                    }

                    return code;
                }
                case Function: {
                    std::string name = tree->attr.name;

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
                        err = true;
                        std::cerr << "[Line " << line << "] Function " << name << " already declared" << std::endl;

                        return new Code(Void);
                    }

                    functions[name] = new Fn(name, variables, type);

                    auto code = new Code(Void);
                    auto body_code = _analyze(tree->child[1]);

                    auto label = "fn_" + name;

                    code->stmt += label + ":\n";
                    code->stmt += body_code->stmt;

                    back_scope();

                    nearest_fn_scope = nullptr;

                    return code;
                }
                case FunctionCall: {
                    std::string name = tree->attr.name;

                    auto & functions = root->functions;

                    if (functions.find(name) == functions.end()) {
                        auto line = tree->line;
                        err = true;
                        std::cerr << "[Line " << line << "] Function " << name << " not declared" << std::endl;

                        return new Code(Void);
                    }

                    auto fn = functions[name];

                    auto args = _analyze(tree->child[0]);

                    auto args_passed = 0;
                    auto t = args;

                    auto code = new Code(fn->return_type);

                    while (t->next) {
                        t = t->next;
                        code->stmt += t->stmt;
                        args_passed++;
                    }

                    auto it = 0;

                    t = args;

                    while (t->next && fn->params.size() > it) {
                        auto param = fn->params[it];
                        auto p = t->next;

                        code->stmt += "param " + p->reg + " " + param->name + "\n";

                        it++;

                        if (param->type != p->type || p->is_array && param->size < 0) {
                            auto line = tree->line;
                            err = true;
                            std::cerr << "[Line " << line << "] Type mismatch in function call" << std::endl;

                            return new Code(Void);
                        }

                        t = p;
                    }

                    if (fn->params.size() != args_passed) {
                        auto line = tree->line;
                        err = true;
                        std::cerr << "[Line " << line << "] Function " << name << " called with wrong number of arguments" << std::endl;

                        return new Code(Void);
                    }

                    auto reg = "$r" + rn();

                    code->stmt += reg + " = call fn_" + name + ", " + std::to_string(args_passed) + "\n";
                    code->reg = reg;

                    return code;
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
                        err = true;
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
                        err = true;
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
                        err = true;
                        std::cerr << "[Line " << line << "] Type mismatch in assignment" << std::endl;

                        return new Code(Void);
                    }

                    auto code = new Code(Void);

                    code->stmt += code_r->stmt;
                    code->stmt += code_l->reg + " = " + code_r->reg + "\n";

                    return code;
                }
                case If: {
                    avance_scope();

                    auto condition_code = _analyze(tree->child[0]);

                    if (condition_code->type != Integer) {
                        auto line = tree->line;
                        err = true;
                        std::cerr << "[Line " << line << "] Type mismatch in if condition" << std::endl;

                        return new Code(Void);
                    }

                    auto code = new Code(Void);

                    auto true_block_code = _analyze(tree->child[1]);

                    auto L1 = "L" + rn();
                    auto L2 = "L" + rn();

                    code->stmt += condition_code->stmt;
                    code->stmt += "if_true " + condition_code->reg + " goto " + L1 + "\n";

                    back_scope();

                    if (tree->child[2]) {
                        avance_scope();
                        auto false_block_code = _analyze(tree->child[2]);
                        code->stmt += false_block_code->stmt;
                        back_scope();
                    }

                    code->stmt += "goto " + L2 + "\n";
                    code->stmt += L1 + ":\n";
                    code->stmt += true_block_code->stmt;
                    code->stmt += L2 + ":\n";

                    return code;
                }
                case While: {
                    avance_scope();

                    auto condition_code = _analyze(tree->child[0]);

                    if (condition_code->type != Integer) {
                        auto line = tree->line;
                        err = true;
                        std::cerr << "[Line " << line << "] Type mismatch in while condition" << std::endl;

                        return new Code(Void);
                    }

                    auto body_code = _analyze(tree->child[1]);

                    back_scope();

                    auto code = new Code(Void);

                    auto L1 = "L" + rn();
                    auto L2 = "L" + rn();

                    code->stmt = L1 + ":\n";
                    code->stmt += condition_code->stmt;
                    code->stmt += "if_false " + condition_code->reg + " goto " + L2 + "\n";
                    code->stmt += body_code->stmt;
                    code->stmt += "goto " + L1 + "\n";

                    return code;
                }
                case Return: {
                    if (!nearest_fn_scope) {
                        auto line = tree->line;
                        err = true;
                        std::cerr << "[Line " << line << "] Error: return outside function" << std::endl;

                        return new Code(Void);
                    }
                    else {
                        if (!tree->child[0]) {
                            return new Code(Void);
                        }

                        auto code = new Code(Void);

                        auto val_code = _analyze(tree->child[0]);

                        code->stmt += val_code->stmt;
                        code->stmt += "return " + val_code->reg + "\n";

                        return code;
                    }
                }
                case StatementList: {
                    auto stmt = tree->sibling;

                    auto code = new Code(Void);

                    while (stmt) {
                        auto stmt_code = _analyze(stmt);
                        code->stmt += stmt_code->stmt;
                        stmt = stmt->sibling;
                    }

                    return code;
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

    if (err) {
        return nullptr;
    }

    auto it = root->functions.find("main");

    if(it == root->functions.end()){
        std::cerr << "Error: Function main not declared" << std::endl;
        return nullptr;
    }

    return code;
}
