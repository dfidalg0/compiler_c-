#ifndef __TOKEN_TYPES__
#define __TOKEN_TYPES__

enum TokenType {
    END_OF_FILE = -1,
    KEYWORD,
    IDENTIFIER,
    BADIDEN,
    NUMBER,
    OPERATOR,
    SEPARATOR,
    LPAREN,
    RPAREN,
    LBRACK,
    RBRACK,
    LBRACE,
    RBRACE,
    COMMENT,
    INVALID
};

#endif // __TOKEN_TYPES__
