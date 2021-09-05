all: lexer

lexer: bin/lex

bin/lex: lexer/main.cpp lexer/src/*.cpp lexer/lib/*.h | bin
	g++ -lfl -I lexer/lib lexer/main.cpp lexer/src/*.cpp -o bin/lex

lexer/main.cpp: lexer/main.l
	flex -o lexer/main.cpp lexer/main.l

bin:
	mkdir bin

clean:
	rm -rf bin
