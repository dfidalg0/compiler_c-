all: compiler

compiler: bin/compiler

lexer: src/lexer.yy.cpp

parser: src/parser.yy.cpp lib/parser.yy.hpp

bin/compiler: main.cpp src/*.cpp lib/*.hpp | bin
	g++ -lfl -I lib main.cpp src/*.cpp -o bin/compiler

src/lexer.yy.cpp: main.l
	flex -o src/lexer.yy.cpp main.l

src/parser.yy.cpp lib/parser.yy.hpp: main.y
	bison -d main.y -o src/parser.yy.cpp && mv src/parser.yy.hpp lib/

bin:
	mkdir bin

clean:
	rm -rf bin
