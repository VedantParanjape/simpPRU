#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>
#include "code_printer.h"
#include "symbol_table.h"
#include "ast.h"
#include "pin_config.h"
#include "firmware_loader.h"

#define VAL(str) #str
#define TOSTRING(str) VAL(str)

void yyerror(const char* s);
extern int yylex();
extern int yyparse();
extern FILE* yyin;

extern int linenumber;
extern ast_node *ast;

const char *argp_program_version = TOSTRING(VERSION_NUMBER);
const char *argp_program_bug_address = "https://github.com/VedantParanjape/simpPRU/issues";
static char *doc = "Compiler for simpPRU, compiles simpPRU code down to PRU-C";
static char args_doc[] = "FILE";

struct arguments 
{
    char output_filename[200];
    char input_filename[200];
    int pruid;
    int verbose;
    int device_id;
    int load;
    int preprocess;
    int test;
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
            else if (atoi(arg) == 2)
            {
                arguments->pruid = 2;
            }
            else if (atoi(arg) == 3)
            {
                arguments->pruid = 3;
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

        case 1111:
            arguments->load = 1;
            break;

        case 2222:
            arguments->preprocess = 1;
            break;

        case 't':
            arguments->test = 1;
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
    {"device", 888, "<device_name>", 0, "Select for which BeagleBoard to compile (pocketbeagle, bbb, bbbwireless, bbai)"},
    {"verbose", 999, 0, 0, "Enable verbose mode (dump symbol table and ast graph"},
    {"load", 1111, 0, 0, "Load generated firmware to /lib/firmware/"},
    {"preprocess", 2222, 0, 0, "Stop after generating intermediate C file"},
    {"test", 't', 0, 0, "Use stub functions for testing PRU specific functions"},
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
    arguments.load = 0;
    arguments.preprocess = 0;
    arguments.test = 0;

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

    int is_rpmsg_used = code_printer(ast, arguments.pruid, arguments.test);
    
    char command[700];
    if (arguments.preprocess == 0)
    {
        if (is_rpmsg_used == 1)
        {
            if (arguments.pruid == 2 || arguments.pruid == 3)
            {
                snprintf(command, 700, "pru-gcc /tmp/temp.c -L%s/lib/ -lprurpmsg%d -o %s.pru%d -mmcu=am335x.pru%d -I%s/include/pru/  -DCONFIG_ENABLE_RPMSG=1 -D__AM572X_ICSS1_PRU%d__", TOSTRING(INSTALL_PATH), arguments.pruid%2, arguments.output_filename, arguments.pruid, arguments.pruid%2, TOSTRING(INSTALL_PATH), arguments.pruid%2);
            }
            else
            {
                snprintf(command, 700, "pru-gcc /tmp/temp.c -L%s/lib/ -lprurpmsg%d -o %s.pru%d -mmcu=am335x.pru%d -I%s/include/pru/  -DCONFIG_ENABLE_RPMSG=1", TOSTRING(INSTALL_PATH), arguments.pruid%2, arguments.output_filename, arguments.pruid, arguments.pruid%2, TOSTRING(INSTALL_PATH));
            }
            
            if (system(command) == -1)
            {
                fprintf(stderr, "\e[31mfatal error:\e[0m unable to call pru-gcc\n");
            }
        }
        else
        {
            snprintf(command, 700, "pru-gcc /tmp/temp.c -o %s.pru%d -mmcu=am335x.pru%d -I%s/include/pru/ -DCONFIG_ENABLE_RPMSG=0", arguments.output_filename, arguments.pruid ,arguments.pruid%2, TOSTRING(INSTALL_PATH));
            if (system(command) == -1)
            {
                fprintf(stderr, "\e[31mfatal error:\e[0m unable to call gcc-pru\n");
            }
        }
    }

    if (arguments.load == 1)
    {
        if (arguments.preprocess == 0 && arguments.test == 0)
        {
            char output_filename_[250];
            snprintf(output_filename_, 250, "%s.pru%d", arguments.output_filename, arguments.pruid);

            if (firmware_loader(output_filename_, arguments.pruid) == -1)
            {
                fprintf(stderr, "\e[31mfatal error:\e[0m unable to load firmware to /lib/firmware/\n");
            }
        }
        else
        {
            fprintf(stderr, "--load cannot be called with -c\n");
        }
    }
}

void yyerror (const char *s) 
{
    fprintf (stderr, "%d : error: %s\n", linenumber, s);
    exit(0);
}
