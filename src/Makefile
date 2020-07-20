compile: bison flex 
	gcc main.c lex.yy.c parser.tab.c symbol_table.c ast.c semantic.c code_printer.c pin_config.c cJSON/cJSON.c vec/vec.c -o compile

cross-arm: bison flex 
	arm-linux-gnueabihf-gcc lex.yy.c parser.tab.c symbol_table.c ast.c semantic.c code_printer.c pin_config.c cJSON/cJSON.c vec/vec.c -o compile

bison: parser.y
	bison -d parser.y

flex: lexer.l parser.tab.c parser.tab.h
	flex lexer.l

clean:
	rm lex.yy.c compile parser.tab.h parser.tab.c

arm: bison flex
