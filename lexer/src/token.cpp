#include <token.h>
#include <iostream>

Token::Token(TokenType type, Position begin, Position end, std::string text):
    _type(type),
    _begin(begin),
    _end(end),
    _text(text) {}

Position Token::begin() { return _begin; }

Position Token::end() { return _end; }

std::string Token::text() { return _text; }

TokenType Token::type() { return _type; }

void Token::print(){
    static const std::string types[] = {
        "if",
        "else",
        "int",
        "return",
        "void",
        "while",
        "identifier",
        "number",
        "add_operator",
        "sub_operator",
        "mul_operator",
        "div_operator",
        "lt_operator",
        "lte_operator",
        "gt_operator",
        "gte_operator",
        "eq_operator",
        "neq_operator",
        "assign_operator",
        "comma",
        "semicolon",
        "left_parenthesis",
        "right_parenthesis",
        "left_bracket",
        "right_bracket",
        "left_braces",
        "right_braces",
        "invalid"
    };

    std::cout
        << "(" << _begin.line() << "," << _begin.col() << ")"
        << ":"
        << "(" << _end.line() << "," << _end.col() << ") - "
        << types[_type];

    switch(_type){
        case IF:
        case ELSE:
        case INT:
        case RETURN:
        case VOID:
        case WHILE:
            std::cout << " keyword" << std::endl;
            break;
        case IDENTIFIER:
            std::cout << ", name = " << _text << std::endl;
            break;
        case NUMBER:
            std::cout << ", value = " << _text << std::endl;
            break;
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case LT:
        case LTE:
        case GT:
        case GTE:
        case EQ:
        case NEQ:
        case ASSIGN:
        case COMMA:
        case SEMI:
        case LPAREN:
        case RPAREN:
        case LBRACK:
        case RBRACK:
        case LBRACE:
        case RBRACE:
            std::cout << ", " << _text << std::endl;
            break;
        case INVALID:
            std::cout << ": " << _text << std::endl;
            break;
    }
}
