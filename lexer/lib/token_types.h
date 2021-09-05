#ifndef __TOKEN_TYPES__
#define __TOKEN_TYPES__

enum TokenType {
    END_OF_FILE = -1,
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    SEPARATOR,
    DELIMITER,
    COMMENT,
    INVALID
};

#endif // __TOKEN_TYPES__
