#ifndef __TOKEN_TYPES__
#define __TOKEN_TYPES__

enum TokenType {
    END_OF_FILE = -1,
    IF,
    ELSE,
    INT,
    RETURN,
    VOID,
    WHILE,
    IDENTIFIER,
    NUMBER,
    ADD,
    SUB,
    MUL,
    DIV,
    LT,
    LTE,
    GT,
    GTE,
    EQ,
    NEQ,
    ASSIGN,
    COMMA,
    SEMI,
    LPAREN,
    RPAREN,
    LBRACK,
    RBRACK,
    LBRACE,
    RBRACE,
    INVALID
};

#endif // __TOKEN_TYPES__
