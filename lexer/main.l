%{
#include <iostream>
#include <lexer.h>
#include "../main.tab.h"

Lexer * current = nullptr;

void set_token_info() {
    current->text << yytext;
    current->line_end = current->line_start;
    current->col_end = current->col_start + yyleng;
}

#define MAKE_TOKEN($type) \
    set_token_info(); \
    return yytokentype::$type;
%}

%x READ_COMMENT

%%

"/*" {
    BEGIN(READ_COMMENT);
    current->col_start += 2;
}

<READ_COMMENT>.|\n {
    if (yytext[0] == '\n') {
        current->col_start = 1;
        current->line_start++;
    }
    else current->col_start++;
}

<READ_COMMENT>"*/" {
    BEGIN(INITIAL);
    current->col_start += 2;
}

<READ_COMMENT><<EOF>> {
    BEGIN(INITIAL);
    return yytokentype::INVALID;
}

if          { MAKE_TOKEN(IF); }
else        { MAKE_TOKEN(ELSE); }
int         { MAKE_TOKEN(INT); }
return      { MAKE_TOKEN(RETURN); }
void        { MAKE_TOKEN(VOID); }
while       { MAKE_TOKEN(WHILE); }

[a-zA-Z]+   { MAKE_TOKEN(IDENTIFIER); }

[0-9]+      { MAKE_TOKEN(NUMBER); }

\+          { MAKE_TOKEN(ADD); }

\-          { MAKE_TOKEN(SUB); }

\*          { MAKE_TOKEN(MUL); }

\/          { MAKE_TOKEN(DIV); }

\<          { MAKE_TOKEN(LT); }

\<=         { MAKE_TOKEN(LTE); }

\>          { MAKE_TOKEN(GT); }

\>=         { MAKE_TOKEN(GTE); }

==          { MAKE_TOKEN(EQ); }

!=          { MAKE_TOKEN(NEQ); }

=           { MAKE_TOKEN(ASSIGN); }

\;          { MAKE_TOKEN(SEMI); }

\,          { MAKE_TOKEN(COMMA); }

\(          { MAKE_TOKEN(LPAREN); }

\)          { MAKE_TOKEN(RPAREN); }

\[          { MAKE_TOKEN(LBRACK); }

\]          { MAKE_TOKEN(RBRACK); }

\{          { MAKE_TOKEN(LBRACE); }

\}          { MAKE_TOKEN(RBRACE); }

\n          { current->line_start++; current->col_start = 1; }
[[:space:]] { current->col_start++; }

<<EOF>>     { return yytokentype::END_OF_FILE; }

.           { MAKE_TOKEN(INVALID); }
%%


