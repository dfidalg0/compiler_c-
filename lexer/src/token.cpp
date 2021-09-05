#include <token.h>

Token::Token(TokenType type, Position begin, Position end, std::string text):
    _type(type),
    _begin(begin),
    _end(end),
    _text(text) {}

Position Token::begin() { return _begin; }

Position Token::end() { return _end; }

std::string Token::text() { return _text; }

TokenType Token::type() { return _type; }
