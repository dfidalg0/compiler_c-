all: compiler

compiler: bin/compiler

bin/compiler: main.cpp parser/index lexer/index.cpp lexer/src/*.cpp lexer/lib/*.h | bin
	g++ -lfl -I lexer/lib main.cpp lexer/index.cpp lexer/src/*.cpp -o bin/compiler

lexer/index.cpp: lexer/main.l
	flex -o lexer/index.cpp lexer/main.l

parser/index: parser/main.y
	bison -d parser/main.y

bin:
	mkdir bin

clean:
	rm -rf bin
