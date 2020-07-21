#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>
#include "code_printer.h"
#include "symbol_table.h"
#include "ast.h"
#include "pin_config.h"

void yyerror(const char* s);
extern int yylex();
extern int yyparse();
extern FILE* yyin;

extern int linenumber;
extern ast_node *ast;

static int pru_id = 0;
static char* output_name = "a.pru";
int debug = 0;

const char * argp_program_version = "v1.0";
const char * argp_program_bug_address = "https://github.com/VedantParanjape/simpPRU/issues";

static int parse_opt(int key, char *arg, struct argp_state *state)
{
    switch (key)
    {
    case 'i':
        yyin = fopen(arg, "r");
        if (yyin == NULL)
        {
            fprintf(stderr, "fatal error: no valid input file\ncompilation terminated\n");
            exit(0);
        }
        break;

    case 777:
        yyin = fopen(arg, "r");
        if (yyin == NULL)
        {
            fprintf(stderr, "fatal error: no valid input file\ncompilation terminated\n");
            exit(0);
        }
        break;

    case 'o':
        output_name = arg;
        snprintf(output_name, 100, "%s.pru%d", arg, pru_id);
        break;

    case 'p':
        if (atoi(arg) == 0)
        {
            pru_id = 0;
        }
        else if (atoi(arg) == 1)
        {
            pru_id = 1;
        }
        else
        {
            fprintf(stderr, "fatal error: incorrect pru id\n");
            exit(0);
        }
        break;

    case 888:
        if (!strcmp(arg, "pocketbeagle"))
        {
            set_device_model(MODEL_POCKETBEAGLE);
        }
        else if (!strcmp(arg, "bbb"))
        {
            set_device_model(MODEL_BEAGLEBONE_BLACK);
        }
        else if (!strcmp(arg, "bbbwireless"))
        {
            set_device_model(MODEL_BEAGLEBONE_BLACK_WIRELESS);
        }
        else if (!strcmp(arg, "bbai"))
        {
            set_device_model(MODEL_BEAGLEBONE_AI);
        }
        else
        {
            fprintf(stderr, "fatal error: incorrect beagleboard model\n");
            exit(0);
        }
        break;

    case 'd':
        debug = 1;  
        break;

    default:
        break;
    }

    return 0;
}

int main(int argc, char** argv)
{
    struct argp_option options[] = 
    {
        {"input", 'i', "file", 0, "Input file to be compiled"},
        {0, 777, 0, 0, 0},
        {"output", 'o', "file", 0, "Place the output into <file>"},
        {"pru", 'p', "pru id", 0, "Select which pru id (0/1) for which program is to be compiled"},
        {"device", 888, "device name", 0, "Select for which BeagleBoard to compile (PocketBeagle, BBB, BBB Wireless, BB AI)"},
        {"debug", 'd', 0, 0, "Enable debug mode (dump symbol table and ast graph"},
        {0}
    };
    struct argp argp = { options, parse_opt };   
    argp_parse(&argp, argc, argv, 0, 0, 0); 

    if (read_device_model() == 0) 
    {
        set_device_model(MODEL_AUTODETECT);
    }   
    init_symbol_table();
    while(yyparse());
    if (debug)
    {
        dump_symbol_table();
        ast_node_dump(ast);
    }

    int is_rpmsg_used = code_printer(ast);
    
    char command[500];
    if (is_rpmsg_used == 1)
    {
        sprintf(command, "pru-gcc ../generated_code/temp.c ../generated_code/pru_rpmsg.c ../generated_code/pru_virtqueue.c -o %s -mmcu=am335x.pru%d -I./../generated_code/include/ -I./../generated_code/include/am335x/ -DCONFIG_ENABLE_RPMSG=1", output_name, pru_id);
        system(command);
    }
    else
    {
        sprintf(command, "pru-gcc ../generated_code/temp.c -o %s -mmcu=am335x.pru%d -I./../generated_code/include/ -I./../generated_code/include/am335x/ -DCONFIG_ENABLE_RPMSG=0", output_name, pru_id);
        system(command);
    }
    // -D_ENABLE_RPMSG=1, here _ENABLE_RPMSG is a macro used, we can define its value like this
}

void yyerror (const char *s) 
{
    fprintf (stderr, "%d : error: %s\n", linenumber, s);
    exit(0);
} 
