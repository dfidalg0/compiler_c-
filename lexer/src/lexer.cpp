#include <lexer.h>
#include <iostream>

extern Lexer * current;
extern FILE *yyin;

Lexer::Lexer(std::string filename) {
    this->file = fopen(filename.c_str(), "r");
    if(this->file == nullptr){
        std::cout << "File not found " << filename << std::endl;
        exit(1);
    }
    current = this;
    yyin = this->file;
    this->line_start = this->col_start = this->line_end = this->col_end = 1;
}

Token Lexer::lex() {

    yytokentype type = (yytokentype) yylex();

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