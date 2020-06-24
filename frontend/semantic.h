#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "symbol_table.h"

int check_function_call(ast_node_function_call *function_call);
int check_function_definition(ast_node_function_def *function_defs);

#endif