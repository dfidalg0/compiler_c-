#ifndef __LEXER__
#define __LEXER__

#include <token.h>
#include <sstream>

extern "C" int yylex();
extern void set_token_info();

class Lexer {
private:
    FILE* file;
    int state;

    unsigned line_start, col_start, line_end, col_end;

    std::stringstream text;

    void set_state(unsigned);

    friend int yylex();
    friend void set_token_info();

public:
    Lexer(std::string);

    Token lex();
};

#endif // __LEXER__
