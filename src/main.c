#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>
#include "code_printer.h"
#include "symbol_table.h"
#include "ast.h"
#include "pin_config.h"

#define VAL(str) #str
#define TOSTRING(str) VAL(str)


void yyerror(const char* s);
extern int yylex();
extern int yyparse();
extern FILE* yyin;

extern int linenumber;
extern ast_node *ast;

const char * argp_program_version = "v1.0";
const char * argp_program_bug_address = "https://github.com/VedantParanjape/simpPRU/issues";
static char * doc = "Compiler for simpPRU, compiles simpPRU code down to PRU-C";
static char args_doc[] = "FILE";

struct arguments 
{
    char output_filename[200];
    char input_filename[200];
    int pruid;
    int verbose;
    int device_id;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;
    switch (key)
    {
        case ARGP_KEY_ARG:
            if (state->arg_num > 1)
            {
                fprintf(stderr, "\e[31merror:\e[0m too many input files\n");
                argp_usage(state);
            }
            snprintf(arguments->input_filename, 200, "%s", arg);
            break;
        
        case ARGP_KEY_END:
            if (state->arg_num < 1)
            {
                fprintf(stderr, "\e[31mfatal error:\e[0m no input file\n");
                argp_usage(state);
            }    
            break;

        case 'o':
            snprintf(arguments->output_filename, 200, "%s", arg);
            break;

        case 'p':
            if (atoi(arg) == 0)
            {
                arguments->pruid = 0;
            }
            else if (atoi(arg) == 1)
            {
                arguments->pruid = 1;
            }
            else
            {
                fprintf(stderr, "\e[31mfatal error:\e[0m incorrect pru id\n");
                argp_usage(state);
            }
            break;

        case 888:
            if (!strcmp(arg, "pocketbeagle"))
            {
                arguments->device_id = MODEL_POCKETBEAGLE;
            }
            else if (!strcmp(arg, "bbb"))
            {
                arguments->device_id = MODEL_BEAGLEBONE_BLACK;
            }
            else if (!strcmp(arg, "bbbwireless"))
            {
                arguments->device_id = MODEL_BEAGLEBONE_BLACK_WIRELESS;
            }
            else if (!strcmp(arg, "bbai"))
            {
                arguments->device_id = MODEL_BEAGLEBONE_AI;
            }
            else
            {
                fprintf(stderr, "\e[31mfatal error:\e[0m incorrect beagleboard model\n");
                argp_usage(state);
            }
            break;

        case 999:
            arguments->verbose = 1;
            break;

        default:
            return ARGP_ERR_UNKNOWN;
            break;
    }

    return 0;
}

struct argp_option options[] = {
    {"output", 'o', "<file>", 0, "Place the output into <file>"},
    {"pru", 'p', "<pru_id>", 0, "Select which pru id (0/1) for which program is to be compiled"},
    {"device", 888, "<device_name>", 0, "Select for which BeagleBoard to compile (PocketBeagle, BBB, BBB Wireless, BB AI)"},
    {"verbose", 999, 0, 0, "Enable verbose mode (dump symbol table and ast graph"},
    {0}
};


int main(int argc, char** argv)
{
    struct arguments arguments;
    arguments.device_id = MODEL_AUTODETECT;
    sprintf(arguments.input_filename, "%s", "");
    sprintf(arguments.output_filename, "%s", "a");
    arguments.pruid = 0;
    arguments.verbose = 0;

    struct argp argp = {options, parse_opt, args_doc, doc};   
    argp_parse(&argp, argc, argv, 0, 0, &arguments); 

    yyin = fopen(arguments.input_filename, "r");
    if (yyin == NULL)
    {
        fprintf(stderr, "\e[31mfatal error:\e[0m no valid input file\ncompilation terminated\n");
        exit(0);
    }

    set_device_model(arguments.device_id);
    init_symbol_table();

    while(yyparse());
    
    if (arguments.verbose)
    {
        dump_symbol_table();
        ast_node_dump(ast);
    }

    int is_rpmsg_used = code_printer(ast);
    
    char command[700];
    if (is_rpmsg_used == 1)
    {
        snprintf(command, 700, "pru-gcc /tmp/temp.c -L%s/lib/ -llibprurpmsg%d -o %s.pru%d -mmcu=am335x.pru%d -I%s/include/simppru-1.0/  -DCONFIG_ENABLE_RPMSG=1", TOSTRING(INSTALL_PATH), arguments.pruid, arguments.output_filename, arguments.pruid, arguments.pruid, TOSTRING(INSTALL_PATH));
        system(command);
    }
    else
    {
        snprintf(command, 700, "pru-gcc /tmp/temp.c -o %s.pru%d -mmcu=am335x.pru%d -I%s/include/simppru-1.0/ -DCONFIG_ENABLE_RPMSG=0", arguments.output_filename, arguments.pruid ,arguments.pruid, TOSTRING(INSTALL_PATH));
        system(command);
    }
}

void yyerror (const char *s) 
{
    fprintf (stderr, "%d : error: %s\n", linenumber, s);
    exit(0);
}
