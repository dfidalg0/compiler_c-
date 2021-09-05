#include <position.h>

Position::Position (unsigned line, unsigned col) : _line(line), _col(col) {}

unsigned Position::line() { return _line; }

unsigned Position::col() { return _col; }
