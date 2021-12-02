%code requires {
    typedef class Token Token;

    #include <iostream>
    #include <token.hpp>
    #include <tree.hpp>

    static TreeNode * savedTree;
    int yyerror(char const *);

    extern "C" int yylex();
}

%union {
    Token * token;
    TreeNode *node;
}

%token <token>
    IF ELSE WHILE RETURN INT VOID
    IDENTIFIER NUMBER
    ASSIGN EQ NEQ LT LTE GT GTE ADD SUB MUL DIV
    LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE COMMA SEMI

%type <node>
    programa declaracao_lista declaracao var_declaracao
    fun_declaracao params param_lista statement expressao_decl
    termo fator soma_expressao iteracao_decl selecao_decl
    composto_decl retorno_decl expressao simples_expressao var
    ativacao arg_lista args param statement_lista statements
%%

programa: declaracao_lista { savedTree = $1; };

declaracao_lista:
    declaracao_lista declaracao {
        TreeNode* t = $1;
        if(t != nullptr){
            while(t->sibling != nullptr)
                t = t->sibling;
            t->sibling = $2;
            $$ = $1;
        } else $$ = $2;
    } |
    declaracao { $$ = $1; };

declaracao:
    var_declaracao { $$ = $1; } |
    fun_declaracao { $$ = $1; } |
    error { $$ = nullptr; };

var_declaracao:
    INT IDENTIFIER SEMI {
        $$ = createExpressionNode(Identifier, $2->begin().line());
        $$->attr.name = copyString($2->text());
        $$->type = Integer;
    } |
    INT IDENTIFIER LBRACK NUMBER RBRACK SEMI {
        $$ = createExpressionNode(Array, $2->begin().line());
        $$->child[0] = createExpressionNode(Constant, $4->begin().line());
        $$->child[0]->attr.pos = std::stoi($4->text());
        $$->attr.name = copyString($2->text());
        $$->type = Integer;
    };


fun_declaracao:
    INT IDENTIFIER LPAREN params RPAREN composto_decl {
        $$ = createExpressionNode(Function, $1->begin().line());
        $$->attr.name = copyString($1->text());
        $$->child[0] = $4;
        $$->child[1] = $6;
    } |
    VOID IDENTIFIER LPAREN params RPAREN composto_decl {
        $$ = createExpressionNode(Function, $1->begin().line());
        $$->attr.name = copyString($1->text());
        $$->child[0] = $4;
        $$->child[1] = $6;
    }

params:
    param_lista {
        $$ = createExpressionNode(ParamsList, -1);
        $$->sibling = $1;
    } |
    VOID {
        $$ = createExpressionNode(ParamsList, -1);
        $$->sibling = nullptr;
    };

param_lista:
    param_lista COMMA param {
        TreeNode* t = $1;
        if(t != nullptr){
            while(t->sibling != nullptr)
                t = t->sibling;
            t->sibling = $3;
            $$ = $1;
        }
        else $$ = $3;
    } |
    param {
        $$ = $1;
        $$->sibling = nullptr;
    };

param:
    INT IDENTIFIER {
        $$ = createExpressionNode(Identifier, $2->begin().line());
        $$->attr.name = copyString($2->text());
        $$->type = Integer;
    } |
    INT IDENTIFIER LBRACK RBRACK {
        $$ = createExpressionNode(Array, $2->begin().line());
        $$->attr.name = copyString($2->text());
        $$->type = Integer;
    };

composto_decl:
    LBRACE statements RBRACE {
        $$ = $2;
    };

statements:
    statement_lista {
        $$ = createStatementNode(StatementList, -1);
        $$->sibling = $1;
    }

statement_lista:
    statement_lista statement {
        TreeNode* t = $1;
        if(t != nullptr){
            while(t->sibling != nullptr)
                t = t->sibling;
            t->sibling = $2;
            $$ = $1;
        }
        else $$ = $2;
    } |
    %empty {
        $$ = nullptr;
    }

statement:
    var_declaracao { $$ = $1; } |
    expressao_decl { $$ = $1; } |
    composto_decl { $$ = $1; } |
    selecao_decl { $$ = $1; } |
    iteracao_decl { $$ = $1; } |
    retorno_decl { $$ = $1; } |
    error { $$ = nullptr; };

expressao_decl:
    expressao SEMI { $$ = $1; } |
    SEMI {};

selecao_decl:
    IF LPAREN expressao RPAREN statement {
        $$ = createStatementNode(If, $1->begin().line());
        $$->child[0] = $3;
        $$->child[1] = $5;
    } |
    IF LPAREN expressao RPAREN statement ELSE statement {
        $$ = createStatementNode(If, $1->begin().line());
        $$->child[0] = $3;
        $$->child[1] = $5;
        $$->child[2] = $7;
    };

iteracao_decl:
    WHILE LPAREN expressao RPAREN statement {
        $$ = createStatementNode(While, $1->begin().line());
        $$->child[0] = $3;
        $$->child[1] = $5;
    };

retorno_decl:
    RETURN SEMI {
        $$ = createStatementNode(Return, $1->begin().line());
    } |
    RETURN expressao SEMI {
        $$ = createStatementNode(Return, $1->begin().line());
        $$->child[0] = $2;
    };

expressao:
    var ASSIGN expressao {
        $$ = createStatementNode(Assign, $2->begin().line());
        $$->child[0] = $1;
        $$->child[1] = $3;
    } |
    simples_expressao { $$ = $1; };

var:
    IDENTIFIER {
        $$ = createExpressionNode(Identifier, $1->begin().line());
        $$->attr.name = copyString($1->text());
    } |
    IDENTIFIER LBRACK expressao RBRACK {
       $$ = createExpressionNode(Array, $1->begin().line());
       $$->child[0] = createExpressionNode(Identifier, $1->begin().line());
       $$->child[0]->attr.name = copyString($1->text());
       $$->child[1] = $3;
    };

simples_expressao:
    soma_expressao LTE soma_expressao {
        $$ = createExpressionNode(Operation, $2->begin().line());
        $$->child[0] = $1;
        $$->child[1] = $3;
        $$->attr.operation = LTE;
    } |
    soma_expressao LT soma_expressao {
        $$ = createExpressionNode(Operation, $2->begin().line());
        $$->child[0] = $1;
        $$->child[1] = $3;
        $$->attr.operation = LT;
    } |
    soma_expressao GT soma_expressao {
        $$ = createExpressionNode(Operation, $2->begin().line());
        $$->child[0] = $1;
        $$->child[1] = $3;
        $$->attr.operation = GT;
    } |
    soma_expressao GTE soma_expressao {
        $$ = createExpressionNode(Operation, $2->begin().line());
        $$->child[0] = $1;
        $$->child[1] = $3;
        $$->attr.operation = GTE;
    } |
    soma_expressao EQ soma_expressao {
        $$ = createExpressionNode(Operation, $2->begin().line());
        $$->child[0] = $1;
        $$->child[1] = $3;
        $$->attr.operation = EQ;
    } |
    soma_expressao NEQ soma_expressao {
        $$ = createExpressionNode(Operation, $2->begin().line());
        $$->child[0] = $1;
        $$->child[1] = $3;
        $$->attr.operation = NEQ;
    } |
    soma_expressao { $$ = $1; };

soma_expressao:
    soma_expressao ADD termo {
        $$ = createExpressionNode(Operation, $2->begin().line());
        $$->child[0] = $1;
        $$->child[1] = $3;
        $$->attr.operation = ADD;
    } |
    soma_expressao SUB termo {
        $$ = createExpressionNode(Operation, $2->begin().line());
        $$->child[0] = $1;
        $$->child[1] = $3;
        $$->attr.operation = SUB;
    } |
    termo { $$ = $1; };

termo:
    termo MUL fator {
        $$ = createExpressionNode(Operation, $2->begin().line());
        $$->child[0] = $1;
        $$->child[1] = $3;
        $$->attr.operation = MUL;
    } |
    termo DIV fator {
        $$ = createExpressionNode(Operation, $2->begin().line());
        $$->child[0] = $1;
        $$->child[1] = $3;
        $$->attr.operation = DIV;
    } |
    fator;

fator:
    LPAREN expressao RPAREN { $$ = $2; } |
    var { $$ = $1; } |
    ativacao { $$ = $1; } |
    NUMBER {
        $$ = createExpressionNode(Constant, $1->begin().line());
        $$->type = Integer;
        $$->attr.val = std::stoi($1->text());
    };

ativacao: IDENTIFIER LPAREN args RPAREN {
    $$ = createExpressionNode(FunctionCall, $1->begin().line());

    $$->child[0] = createExpressionNode(Identifier, $1->begin().line());
    $$->attr.name = copyString($1->text());
    $$->type = Integer;

    $$->child[1] = $3;
};

args:
    arg_lista {
        $$ = createExpressionNode(ParamsList, -1);
        $$->sibling = $1;
    } |
    %empty {
        $$ = createExpressionNode(ParamsList, -1);
        $$->sibling = nullptr;
    }

arg_lista:
    arg_lista COMMA expressao {
        TreeNode* t = $1;
        if(t != nullptr){
            while(t->sibling != nullptr)
                t = t->sibling;
            t->sibling = $3;
            $$ = $1;
        }
        else $$ = $3;
    } |
    expressao {
        $$ = $1;
        $$->sibling = nullptr;
    }

%%
int yyerror(char const * err) {
    std::cerr << err << std::endl;
    return 0;
};

TreeNode* getTree(){
    yyparse();
    return savedTree;
}
