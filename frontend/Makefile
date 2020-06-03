compile: bison flex
	gcc lex.yy.c parser.tab.c -o compile

bison: parser.y
	bison -d parser.y

flex: lexer.l parser.tab.c parser.tab.h
	flex lexer.l

clean:
	rm lex.yy.c compile parser.tab.h parser.tab.c
