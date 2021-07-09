#include "semantic.h"

int check_function_call(ast_node_function_call *function_call)
{   
    if (function_call->args != NULL)
    {   
        if (function_call->args->arguments.length == function_call->symbol_entry->params.length)
        {
            int return_value = 1;
            for (int i = 0; i < function_call->args->arguments.length; i++)
            {
                int expected_dt = function_call->symbol_entry->params.data[i]->data_type;

                if (function_call->args->arguments.data[i]->node_type == AST_NODE_ARITHMETIC_EXP
                    && (expected_dt == DT_BOOLEAN))
                {
                    return_value = 0;
                    break;
                }
                else if (function_call->args->arguments.data[i]->node_type == AST_NODE_BOOLEAN_EXP
                    && expected_dt == DT_INTEGER || expected_dt == DT_CHAR_)
                {
                    return_value = 0;
                    break;
                }
            }
            return return_value;
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

int check_function_definition(ast_node_function_def *function_defs)
{
    if (function_defs->symbol_entry != NULL)
    {
        int expected_dt = function_defs->symbol_entry->data_type;

        if (function_defs->return_statment == NULL && expected_dt == DT_VOID_)
        {
            return 1;
        }
        else if (function_defs->return_statment->node_type == AST_NODE_ARITHMETIC_EXP
                && (expected_dt == DT_INTEGER || expected_dt == DT_CHAR_))
        {
            return 1;
        }
        else if (function_defs->return_statment->node_type == AST_NODE_BOOLEAN_EXP && expected_dt == DT_BOOLEAN)
        {
            return 1;
        }
    }

    return -1;
}