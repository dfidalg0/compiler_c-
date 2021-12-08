%{
#include <iostream>
#include <position.hpp>
#include <parser.yy.hpp>
#include <globals.hpp>

void set_token_info(yytokentype type) {
    line_end = line_start;
    col_end = col_start + yyleng;

    yylval.token = new Token(
        type,
        Position(line_start, col_start),
        Position(line_end,   col_end  ),
        std::string(yytext)
    );

    line_start = line_end;
    col_start = col_end;
}

#define MAKE_TOKEN($type) \
    set_token_info(yytokentype::$type); \
    return yytokentype::$type;
%}

%x READ_COMMENT

%%

"/*" {
    BEGIN(READ_COMMENT);
    col_start += 2;
}

<READ_COMMENT>.|\n {
    if (yytext[0] == '\n') {
        col_start = 1;
        line_start++;
    }
    else col_start++;
}

<READ_COMMENT>"*/" {
    BEGIN(INITIAL);
    col_start += 2;
}

<READ_COMMENT><<EOF>> {
    BEGIN(INITIAL);
    return yytokentype::YYEOF;
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

\n          { line_start++; col_start = 1; }
[[:space:]] { col_start++; }

<<EOF>>     { return yytokentype::YYEOF; }

.           { MAKE_TOKEN(YYerror); }
%%
