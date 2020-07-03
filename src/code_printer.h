#ifndef CODE_PRINTER_H
#define CODE_PRINTER_H

#include "ast.h"
#include "symbol_table.h"
#include "pin_config.h"
#include "vec/vec.h"

#include <stdio.h>
#include <stdlib.h>

#define _OPR_ADD " + " 
#define _OPR_SUB " - " 
#define _OPR_MUL " * " 
#define _OPR_DIV " / " 

#define _OPR_GT " > "  
#define _OPR_LT " < " 
#define _OPR_EQ " == " 
#define _OPR_NE " != " 
#define _OPR_GE " >= " 
#define _OPR_LE " <= " 

#define _OPR_BW_NOT  " ~ " 
#define _OPR_BW_AND  " & " 
#define _OPR_BW_OR   " | " 
#define _OPR_LGL_NOT " ! " 
#define _OPR_LGL_AND " && " 
#define _OPR_LGL_OR  " | " 

#define _OPR_ASSIGNMENT " = " 

#define _DT_INT_ "int"
#define _DT_BOOL_ "bool"
#define _DT_VOID_ "void"

#define BEGIN "#include \"resource_table.h\"\n#include <pru/io.h>\n\n"
#define MAIN "int main(void)\n{\n"
#define END "\n\treturn 0;\n}\n"
#define DIGITAL_WRITE "void digital_write(int pin, int value)\n{\n\tif(value == 0) write_r30(read_r31()|0);\n\telse write_r30(read_r31()|((uint32_t) 1 << (pin%32)));\n}\n\n"
#define DIGITAL_READ "int digital_read(int pin)\n{\n\tif (read_r31() & ((uint32_t) 1 << (pin%32))) return 1;\n\telse return 0;\n}\n\n"
#define START_COUNTER "void start_counter()\n{\n\tPRU_CTRL.CYCLE = 0;\n\tPRU_CTRL.CONTROL_bit.COUNTER_ENABLE = 1;\n}\n\n"
#define STOP_COUNTER "void stop_counter()\n{\n\tPRU_CTRL.CONTROL_bit.COUNTER_ENABLE = 0;\n}\n\n"
#define READ_COUNTER "int read_counter()\n{\n\treturn PRU_CTRL.CYCLE;\n}\n\n"

void ast_compound_statement_printer(ast_node_compound_statement *cmpd_stmt, FILE* handle, int is_func_def);
void ast_declaration_printer(ast_node_declaration *decl, FILE* handle);
void ast_assignment_printer(ast_node_assignment *assg, FILE* handle);
void ast_expression_printer(ast_node_expression *node, FILE* handle);
void ast_conditional_if_printer(ast_node_conditional_if *node, FILE* handle);
void ast_loop_for_printer(ast_node_loop_for *node, FILE* handle);
void ast_loop_while_printer(ast_node_loop_while *node, FILE* handle);
void ast_function_call_printer(ast_node_function_call *fc, FILE* handle);
void ast_utility_function_call_printer(ast_node_utility_function_call *ufc, FILE* handle);
void ast_function_definition(ast_node_function_def *def, FILE* handle);
void code_printer(ast_node* ast);

#endif