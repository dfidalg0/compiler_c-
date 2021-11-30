#ifndef __TOKEN__
#define __TOKEN__

#include <position.h>
#include <token_types.h>
#include <string>

class Token {
private:
    Position _begin;
    Position _end;
    std::string _text;
    TokenType _type;
public:
    Token(TokenType type, Position begin, Position end, std::string text);
    TokenType type();
    Position begin();
    Position end();
    std::string text();
    void print();
};

#endif // __TOKEN__
