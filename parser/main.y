%{


%}

%token IF ELSE WHILE RETURN INT VOID 
%token IDENTIFIER NUMBER
%token ASSIGN EQ NEQ LT LTE GT GTE ADD SUB MUL DIV
%token LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE COMMA SEMI
%token INVALID

%%

programa: declaracao-lista;

declaracao-lista: declaracao-lista declaracao | 
                  declaracao;

declaracao: var-declaracao | 
            fun-declaracao;

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

statement: expressao-decl | 
           composto-decl | 
           selecao-decl |
           iteracao-decl |
           retorno-decl;

expressao-decl: expressao SEMI | SEMI;

selecao-decl: IF LPAREN expressao RPAREN statement |
              IF LPAREN expressao RPAREN statement ELSE statement;

iteracao-decl: WHILE LPAREN expressao RPAREN statement;

retorno-decl: RETURN SEMI | 
              RETURN expressao SEMI;

expressao: var ASSIGN expressao | 
           simples-expressao;

var: IDENTIFIER |
     IDENTIFIER LBRACK expressao RBRACK;

simples-expressao: soma-expressao relacional soma-expressao |
                   soma-expressao;

relacional: LTE | LT | GT | GTE | EQ | NEQ;

soma-expressao: soma-expressao soma termo |
                termo;

soma: ADD | SUB;

termo: termo mult fator | fator;

mult: MUL | DIV;

fator: LPAREN expressao RPAREN | var | ativacao | NUMBER;

ativacao: IDENTIFIER LPAREN args RPAREN;

args: arg-lista | %empty;

arg-lista: arg-lista COMMA expressao | expressao

%%

