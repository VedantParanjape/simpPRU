compile: bison flex 
	gcc lex.yy.c parser.tab.c symbol_table.c ast.c semantic.c code_printer.c vec/vec.c -o compile

cross-arm: bison flex 
	arm-linux-gnueabihf-gcc lex.yy.c parser.tab.c symbol_table.c ast.c semantic.c code_printer.c vec/vec.c -o compile
	pru-gcc ../generated_code/temp.c -o out.pru -mmcu=am335x.pru0

bison: parser.y
	bison -d parser.y

flex: lexer.l parser.tab.c parser.tab.h
	flex lexer.l

clean:
	rm lex.yy.c compile parser.tab.h parser.tab.c out.pru

arm: bison flex
