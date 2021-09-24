%{
#include <iostream>
#include <lexer.h>

Lexer * current = nullptr;

void set_token_info() {
    current->text << yytext;
    current->line_end = current->line_start;
    current->col_end = current->col_start + yyleng;
}
%}

%x READ_COMMENT

keyword           (if|else|int|return|void|while)
operator          (\+|\-|\*|\/|<|<=|>|>=|==|!=|=)
separator         (;|,)
identifier        [a-zA-Z]+
number            [0-9]+
malformed_identifier ({number}{identifier})
left_parenthesis  (\()
right_parenthesis (\))
left_bracket      (\[)
right_bracket     (\])
left_braces       (\{)
right_braces      (\})

%%
"/*" {
    BEGIN(READ_COMMENT);
    current->text << yytext;
}

<READ_COMMENT>.|\n {
    current->text << yytext;
    if (yytext[0] == '\n') {
        current->col_end = 1;
        current->line_end++;
    }
    else current->col_end++;
}

<READ_COMMENT>"*/" {
    current->text << yytext;
    BEGIN(INITIAL);

    current->col_end += 2;

    return TokenType::COMMENT;
}

<READ_COMMENT><<EOF>> {
    BEGIN(INITIAL);
    return TokenType::INVALID;
}

{keyword} {
    set_token_info();
    return TokenType::KEYWORD;
}

{identifier} {
    set_token_info();
    return TokenType::IDENTIFIER;
}

{malformed_identifier} {
    set_token_info();
    return TokenType::BADIDEN;
}

{number} {
    set_token_info();
    return TokenType::NUMBER;
}

{operator} {
    set_token_info();
    return TokenType::OPERATOR;
}

{separator} {
    set_token_info();
    return TokenType::SEPARATOR;
}


{left_parenthesis} {
    set_token_info();
    return TokenType::LPAREN;
}

{right_parenthesis} {
    set_token_info();
    return TokenType::RPAREN;
}

{left_bracket} {
    set_token_info();
    return TokenType::LBRACK;
}

{right_bracket} {
    set_token_info();
    return TokenType::RBRACK;
}

{left_braces} {
    set_token_info();
    return TokenType::LBRACE;
}

{right_braces} {
    set_token_info();
    return TokenType::RBRACE;
}

\n          { current->line_start++; current->col_start = 1; }
[[:space:]] { current->col_start++; }

<<EOF>>     { return TokenType::END_OF_FILE; }

. {
    set_token_info();
    return TokenType::INVALID;
}
%%

int main(int argc, char **argv) {
    
    if(argc != 2) {
        std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
        exit(1);
    }

    auto lexer = Lexer(argv[1]);

    auto token = lexer.lex();

    while (token.type() != TokenType::END_OF_FILE) {
        token.print_token();
        token = lexer.lex();
    }

    return 0;
}
