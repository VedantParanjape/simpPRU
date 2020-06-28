#ifndef CODE_PRINTER_H
#define CODE_PRINTER_H

#include "ast.h"
#include "symbol_table.h"
#include "vec/vec.h"
#include <stdio.h>
#include <stdlib.h>

#define _OPR_ADD " + " 
#define _OPR_SUB " - " 
#define _OPR_MUL " * " 
#define _OPR_DIV " / " 

#define _OPR_GT " > "  
#define _OPR_LT " < " 
#define _OPR_EQ " = " 
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

#define BEGIN "#include \"resource_table.h\"\n\nint main(void)\n\{\n"
#define END "\n\treturn 0;\n}\n"

void ast_declaration_printer(ast_node_declaration *decl, FILE* handle);
void ast_assignment_printer(ast_node_assignment *assg, FILE* handle);
void ast_expression_printer(ast_node_expression* node, FILE* handle);
void ast_function_call_printer(ast_node_function_call* fc, FILE* handle);
void code_printer(ast_node* ast);

#endif