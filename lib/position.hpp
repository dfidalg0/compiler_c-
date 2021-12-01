#ifndef __POSITION__
#define __POSITION__

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
