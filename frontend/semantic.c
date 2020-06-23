#include "semantic.h"

int check_function_call(ast_node_function_call *function_call)
{   
    if (function_call->args != NULL)
    {   
        if (function_call->args->arguments.length == function_call->symbol_entry->params.length)
        {
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