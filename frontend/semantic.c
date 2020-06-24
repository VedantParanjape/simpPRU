#include "semantic.h"

int check_function_call(ast_node_function_call *function_call)
{   
    if (function_call->args != NULL)
    {   
        if (function_call->args->arguments.length == function_call->symbol_entry->params.length)
        {
            for (int i = 0; i < function_call->args->arguments.length; i++)
            {
                int dt = 0;

                if (function_call->args->arguments.data[i]->node_type == AST_NODE_ARITHMETIC_EXP)
                {
                    dt = AST_DT_INT;
                }
                else
                {
                    dt = AST_DT_BOOL;
                }

                if (function_call->symbol_entry->params.data[i]->data_type != dt)
                {
                    return -1;
                }
            }
            return 1;
        }
    }
    else
    {
        if (function_call->symbol_entry->params.length == 0)
        {
            return 1;
        }
    }

    return -1;
}