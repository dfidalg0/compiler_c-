#ifndef __TOKEN__
#define __TOKEN__

#include <position.hpp>
#include <string>
#include <parser.yy.hpp>

class Token {
private:
    Position _begin;
    Position _end;
    std::string _text;
    yytokentype _type;
public:
    Token(yytokentype type, Position begin, Position end, std::string text);
    yytokentype type();
    Position begin();
    Position end();
    std::string text();
    void print();
};

#endif // __TOKEN__
