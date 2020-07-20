#include <stdio.h>
#include <stdlib.h>
#include "code_printer.h"
#include "symbol_table.h"
#include "ast.h"

void yyerror(const char* s);
extern int yylex();
extern int yyparse();
extern FILE* yyin;

extern int linenumber;
extern ast_node *ast;

int main(int argc, char** argv)
{
    FILE* fhandle = fopen(argv[1], "r");
    yyin = fhandle;
    init_symbol_table();

    while(yyparse());

    dump_symbol_table();
    /* printf("%d",ast->node_type); */

    ast_node_dump(ast);
    int is_rpmsg_used = code_printer(ast);
    
    if (is_rpmsg_used == 1)
    {
        system("pru-gcc ../generated_code/temp.c ../generated_code/pru_rpmsg.c ../generated_code/pru_virtqueue.c -o out.pru -mmcu=am335x.pru0 -I./../generated_code/include/ -I./../generated_code/include/am335x/ -DCONFIG_ENABLE_RPMSG=1");
    }
    else
    {
        system("pru-gcc ../generated_code/temp.c -o out.pru -mmcu=am335x.pru0 -I./../generated_code/include/ -I./../generated_code/include/am335x/ -DCONFIG_ENABLE_RPMSG=0");
    }
    // -D_ENABLE_RPMSG=1, here _ENABLE_RPMSG is a macro used, we can define its value like this
}

void yyerror (const char *s) 
{
    fprintf (stderr, "%d : error: %s\n", linenumber, s);
    exit(0);
} 
