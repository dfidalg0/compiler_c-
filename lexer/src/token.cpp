#include <token.h>
#include <iostream>

std::string types[] = {
            "keyword",
            "identifier",
            "malformed_identifier",
            "number",
            "operator",
            "separator",
            "left_parenthesis",
            "right_parenthesis",
            "left_bracket",
            "right_bracket",
            "left_braces",
            "right_braces",
            "comment",
            "invalid"
};

Token::Token(TokenType type, Position begin, Position end, std::string text):
    _type(type),
    _begin(begin),
    _end(end),
    _text(text) {}

Position Token::begin() { return _begin; }

Position Token::end() { return _end; }

std::string Token::text() { return _text; }

TokenType Token::type() { return _type; }

void Token::print_token(){
    if(_type == COMMENT) return;

    std::cout << _begin.line() << ", " << _begin.col()
              << "-" << _end.col() << ": " << types[_type];
    switch(_type){
        case OPERATOR:
        case SEPARATOR:
        case LPAREN:
        case RPAREN:
        case LBRACK:
        case RBRACK:
        case LBRACE:
        case RBRACE:
        case KEYWORD:
            std::cout << ", " << _text << std::endl;
            break;
        case IDENTIFIER:
        case BADIDEN:
            std::cout << ", name = " << _text << std::endl;
            break;
        case NUMBER:
            std::cout << ", val = " << _text << std::endl;
            break;
        case INVALID:
            std::cout << ": " << _text << std::endl;
            break;
    }
}
