%{

#include "parser/lib/parse.h"

#define YYSTYPE TreeNode *
static char * savedName; 
static int savedLineNo; 
static TreeNode * savedTree; 
static int yylex(void);
int yyerror(char *s);

%}

%token IF ELSE WHILE RETURN INT VOID 
%token IDENTIFIER NUMBER
%token ASSIGN EQ NEQ LT LTE GT GTE ADD SUB MUL DIV
%token LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE COMMA SEMI
%token INVALID END_OF_FILE

%%

programa: declaracao-lista
          { savedTree = $1; };

declaracao-lista: declaracao-lista declaracao 
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

declaracao: var-declaracao { $$ = $1; } | 
            fun-declaracao { $$ = $1; } |
            error { $$ = NULL; };

var-declaracao: tipo-especificador IDENTIFIER SEMI | 
                tipo-especificador IDENTIFIER LBRACK NUMBER RBRACK SEMI;

tipo-especificador: INT | VOID;

fun-declaracao: tipo-especificador IDENTIFIER LPAREN params RPAREN composto-decl;

params: param-lista | VOID;

param-lista: param-lista COMMA param | 
             param;

param: tipo-especificador IDENTIFIER | 
       tipo-especificador IDENTIFIER LBRACK RBRACK;

composto-decl: LBRACE local-declaracoes statement-lista RBRACE;

local-declaracoes: local-declaracoes var-declaracao | %empty;

statement-lista: statement-lista statement | %empty;

statement: expressao-decl { $$ = $1; } | 
           composto-decl { $$ = $1; } | 
           selecao-decl { $$ = $1; } |
           iteracao-decl { $$ = $1; } |
           retorno-decl { $$ = $1; } |
           error { $$ = NULL; };

expressao-decl: expressao SEMI | SEMI;

selecao-decl: IF LPAREN expressao RPAREN statement 
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

iteracao-decl: WHILE LPAREN expressao RPAREN statement
               {
                 $$ = newStmtNode(RepeatK);
                 $$->child[0] = $3;
                 $$->child[1] = $5;
               };

retorno-decl: RETURN SEMI | 
              RETURN expressao SEMI;

expressao: var ASSIGN expressao | 
           simples-expressao;

var: IDENTIFIER |
     IDENTIFIER LBRACK expressao RBRACK;

simples-expressao: soma-expressao relacional soma-expressao |
                   soma-expressao;

relacional: LTE | LT | GT | GTE | EQ | NEQ;

soma-expressao: soma-expressao ADD termo 
                  {
                    $$ = newExpNode(OpK);
                    $$->child[0] = $1;
                    $$->child[1] = $3;
                    $$->attr.op = ADD;
                  } 
                  | soma-expressao SUB termo
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

args: arg-lista | %empty;

arg-lista: arg-lista COMMA expressao | expressao

%%

