#ifndef __TOKEN__
#define __TOKEN__

#include <position.hpp>
#include <string>

class Token {
private:
    Position _begin;
    Position _end;
    std::string _text;
    int _type;
public:
    Token(int type, Position begin, Position end, std::string text);
    int type();
    Position begin();
    Position end();
    std::string text();
    void print();
};

#endif // __TOKEN__
