compile: lexer
	@g++ lex.yy.c -o lexer
	
lexer: lexer.l
	@flex lexer.l

clean: lex.yy.c lexer	
	rm lex.yy.c lexer