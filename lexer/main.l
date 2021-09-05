%{
#include <iostream>
#include <lexer.h>

extern "C" int yylex();

Lexer * current = nullptr;

void set_token_info() {
    current->text << yytext;
    current->line_end = current->line_start;
    current->col_end = current->col_start + strlen(yytext);
}
%}

%x READ_COMMENT

keyword     (if|else|int|return|void|while)
operator    (\+|\-|\*|\/|<|<=|>|>=|==|!=|=)
separator   (;|,)
identifier  [a-zA-Z]+
number      [0-9]+
delimiter   (\(|\)|\{|\}|\[|\])

%%
"/*" {
    current->set_state(READ_COMMENT);
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
    current->set_state(INITIAL);

    current->col_end += 2;

    return TokenType::COMMENT;
}

<READ_COMMENT><<EOF>> {
    current->set_state(INITIAL);
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

{number} {
    set_token_info();
    return TokenType::IDENTIFIER;
}

{operator} {
    set_token_info();
    return TokenType::OPERATOR;
}

{separator} {
    set_token_info();
    return TokenType::SEPARATOR;
}

{delimiter} {
    set_token_info();
    return TokenType::DELIMITER;
}

\n          { current->line_start++; current->col_start = 1; }
[[:space:]] { current->col_start++; }

<<EOF>>     { return TokenType::END_OF_FILE; }

. {
    set_token_info();
    return TokenType::INVALID;
}
%%

Lexer::Lexer(std::string filename) {
    this->file = fopen(filename.c_str(), "r");
    this->state = INITIAL;
    this->line_start = this->col_start = this->line_end = this->col_end = 1;
}

void Lexer::set_state(unsigned state) {
    this->state = state;
    if (current == this) {
        BEGIN(state);
    }
}

Token Lexer::lex() {
    if (current != this) {
        current = this;
        yyin = this->file;
        BEGIN(this->state);
    }

    TokenType type = (TokenType) yylex();

    auto text = this->text.str();

    this->text.str("");

    auto token = Token(
        type,
        Position(this->line_start, this->col_start),
        Position(this->line_end,   this->col_end  ),
        text
    );

    this->line_start = this->line_end;
    this->col_start = this->col_end;

    return token;
}

int main() {
    auto lexer = Lexer("./examples/sort.c");

    auto token = lexer.lex();

    while (token.type() != TokenType::END_OF_FILE) {
        std::string types[] = {
            "keyword",
            "identifier",
            "number",
            "operator",
            "separator",
            "delimiter",
            "comment",
            "invalid"
        };

        auto type  = types[token.type()];
        auto text  = token.text();
        auto begin = token.begin();
        auto end   = token.end();

        std::cout
            << "Type: "  << type         << '\n'
            << "Begin: " << begin.line() << ':'    << begin.col() << '\n'
            << "End: "   << end.line()   << ':'    << end.col()   << '\n'
            << "Text: "  << text         << "\n\n";

        token = lexer.lex();
    }

    return 0;
}
