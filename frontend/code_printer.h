#ifndef CODE_PRINTER_H
#define CODE_PRINTER_H

#include "ast.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>

#define BEGIN "#include \"resource_table.h\"\n\nint main(void)\n\{\n"
#define END "\n\treturn 0;\n}\n"
#define EMPTY_STATEMENT ";\n"
#define NODE_DECLARATION(data_type, id, val) (data_type) (id) ":=" (val) ";"

void code_printer(ast_node* ast);
void ast_node_action(ast_node* node, int node_type, FILE* handle);

#endif