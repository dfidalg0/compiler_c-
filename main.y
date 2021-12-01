%code requires {
    typedef class Token Token;

    #include <iostream>
    #include <parse.hpp>
    #include <token.hpp>

    int yyerror(char const *);

    extern "C" int yylex();
}

%union {
    Token * token;
}

%token <token>
    IF ELSE WHILE RETURN INT VOID
    IDENTIFIER NUMBER
    ASSIGN EQ NEQ LT LTE GT GTE ADD SUB MUL DIV
    LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE COMMA SEMI

%type <token>
    prog

%%

prog: IF

/* programa: declaracao_lista
          { savedTree = $1; };

declaracao_lista: declaracao_lista declaracao
                  {
                    YYSTYPE t = $1;
                    if(t != NULL){
                      while(t->sibling != NULL)
                        t = t->sibling;
                      t->sibling = $2;
                      $$ = $1;
                    } else $$ = $2;
                  } |
                  declaracao { $$ = %1; };

declaracao: var_declaracao { $$ = $1; } |
            fun_declaracao { $$ = $1; } |
            error { $$ = NULL; };

var_declaracao: tipo_especificador IDENTIFIER SEMI |
                tipo_especificador IDENTIFIER LBRACK NUMBER RBRACK SEMI;

tipo_especificador: INT | VOID;

fun_declaracao: tipo_especificador IDENTIFIER LPAREN params RPAREN composto_decl;

params: param_lista | VOID;

param_lista: param_lista COMMA param |
             param;

param: tipo_especificador IDENTIFIER |
       tipo_especificador IDENTIFIER LBRACK RBRACK;

composto_decl: LBRACE local_declaracoes statement_lista RBRACE;

local_declaracoes: local_declaracoes var_declaracao | %empty;

statement_lista: statement_lista statement | %empty;

statement: expressao_decl { $$ = $1; } |
           composto_decl { $$ = $1; } |
           selecao_decl { $$ = $1; } |
           iteracao_decl { $$ = $1; } |
           retorno_decl { $$ = $1; } |
           error { $$ = NULL; };

expressao_decl: expressao SEMI | SEMI;

selecao_decl: IF LPAREN expressao RPAREN statement
              {
                $$ = newStmtNode(IfK);
                $$->child[0] = $3;
                $$->child[1] = $5;
              } |
              IF LPAREN expressao RPAREN statement ELSE statement
              {
                $$ = newStmtNode(IfK);
                $$->child[0] = $3;
                $$->child[1] = $5;
                $$->child[2] = $7;
              };

iteracao_decl: WHILE LPAREN expressao RPAREN statement
               {
                 $$ = newStmtNode(RepeatK);
                 $$->child[0] = $3;
                 $$->child[1] = $5;
               };

retorno_decl: RETURN SEMI |
              RETURN expressao SEMI;

expressao: var ASSIGN expressao |
           simples_expressao;

var: IDENTIFIER |
     IDENTIFIER LBRACK expressao RBRACK;

simples_expressao: soma_expressao relacional soma_expressao |
                   soma_expressao;

relacional: LTE | LT | GT | GTE | EQ | NEQ;

soma_expressao: soma_expressao ADD termo
                  {
                    $$ = newExpNode(OpK);
                    $$->child[0] = $1;
                    $$->child[1] = $3;
                    $$->attr.op = ADD;
                  }
                  | soma_expressao SUB termo
                  {
                    $$ = newExpNode(OpK);
                    $$->child[0] = $1;
                    $$->child[1] = $3;
                    $$->attr.op = SUB;
                  } |
                termo { $$ = $1; };

termo: termo MUL fator
       {
         $$ = newExpNode(Opk);
         $$->child[0] = $1;
         $$->child[1] = $3;
         $$->attr.op = MUL
       }
       | termo DIV fator
       {
         $$ = newExpNode(Opk);
         $$->child[0] = $1;
         $$->child[1] = $3;
         $$->attr.op = DIV
       } | fator;

fator: LPAREN expressao RPAREN | var | ativacao | NUMBER;

ativacao: IDENTIFIER LPAREN args RPAREN;

args: arg_lista | %empty;

arg_lista: arg_lista COMMA expressao | expressao */

%%
int yyerror(char const * err) {
    std::cerr << err << std::endl;
    return 0;
};
