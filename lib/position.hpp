#ifndef __POSITION__
#define __POSITION

class Position {
private:
    unsigned _line;
    unsigned _col;
public:
    Position(unsigned line, unsigned col);
    unsigned line();
    unsigned col();
};

#endif
