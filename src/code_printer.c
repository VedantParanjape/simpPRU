#include "code_printer.h"

static int rpmsg_artifact_found = 0;
static char pru_id_[5] = "pru0";

void ast_compound_statement_printer(ast_node_compound_statement *cmpd_stmt, FILE* handle, int is_func_def)
{
    int i = 0;
    ast_node_statements *temp;

    fprintf(handle, "%s", "{\n");
    vec_foreach(&cmpd_stmt->child_nodes, temp, i)
    {
        switch(temp->node_type)
        {
            case AST_NODE_COMPOUND_STATEMENT:
                ast_compound_statement_printer(((ast_node_statements*)temp)->child_nodes.compound_statement, handle, 0);
                break;
            
            case AST_NODE_DECLARATION:
                ast_declaration_printer(((ast_node_statements*)temp)->child_nodes.declaration, handle);
                break;

            case AST_NODE_ARRAY_DECLARATION:
                ast_array_declaration_printer(((ast_node_statements*)temp)->child_nodes.array_declaration, handle);
                break;
            
            case AST_NODE_ASSIGNMENT:
                ast_assignment_printer(((ast_node_statements*)temp)->child_nodes.assignment, handle);
                break;

            case AST_NODE_ARRAY_ASSIGNMENT:
                ast_array_assignment_printer(((ast_node_statements*)temp)->child_nodes.array_assignment, handle);
                break;
            
            case AST_NODE_CONDITIONAL_IF:
                ast_conditional_if_printer(((ast_node_statements*)temp)->child_nodes.if_else, handle); 
                break;
        
            case AST_NODE_LOOP_FOR:
                ast_loop_for_printer(((ast_node_statements*)temp)->child_nodes.loop_for, handle);
                break;
            
            case AST_NODE_LOOP_WHILE:
                ast_loop_while_printer(((ast_node_statements*)temp)->child_nodes.loop_while, handle);
                break;
            
            case AST_NODE_FUNC_CALL:
                fprintf(handle, "%s", "\t");
                ast_function_call_printer(((ast_node_statements*)temp)->child_nodes.function_call, handle);
                fprintf(handle, "%s", ";\n");
                break;

            case AST_NODE_LOOP_BREAK:
                if (((ast_node_statements*)temp)->child_nodes.loop_control->node_type == AST_NODE_LOOP_BREAK)
                {
                    fprintf(handle, "\t%s;\n", "break");
                }
                break;

            case AST_NODE_LOOP_CONTINUE:
                if (((ast_node_statements*)temp)->child_nodes.loop_control->node_type == AST_NODE_LOOP_CONTINUE)
                {
                    fprintf(handle, "\t%s;\n", "continue");
                }
                break;
                
            case AST_NODE_DIGITAL_READ_CALL:
            case AST_NODE_DIGITAL_WRITE_CALL:
            case AST_NODE_PWM_CALL:
            case AST_NODE_DELAY_CALL:
            case AST_NODE_START_COUNTER_CALL:
            case AST_NODE_STOP_COUNTER_CALL:
            case AST_NODE_READ_COUNTER_CALL:
            case AST_NODE_INIT_RPMSG_CALL:
            case AST_NODE_RECV_RPMSG_CALL:
            case AST_NODE_SEND_RPMSG_CALL:
                fprintf(handle, "%s", "\t");
                ast_utility_function_call_printer(((ast_node_statements*)temp)->child_nodes.utility_function_call, handle);
                fprintf(handle, "%s", ";\n");
                break;
            
            case AST_NODE_PRINT_STRING_FUNCTION_CALL:
                fprintf(handle, "%s", "\t");
                ast_print_string_function_call_printer(((ast_node_statements*)temp)->child_nodes.print_string_function_call, handle);
                fprintf(handle, "%s", ";\n");
                break;
            
            case AST_NODE_PRINT_EXP_FUNCTION_CALL:
                fprintf(handle, "%s", "\t");
                ast_print_expression_function_call_printer(((ast_node_statements*)temp)->child_nodes.print_expression_function_call, handle);
                fprintf(handle, "%s", ";\n");
                break;
        }
    }
    if (is_func_def == 0)
    {
        fprintf(handle, "%s", "}\n");
    }
}

void ast_declaration_printer(ast_node_declaration *decl, FILE* handle)
{
    if (decl != NULL && handle != NULL)
    {
        if (decl->expression == NULL)
        {
            if (decl->symbol_entry->data_type == DT_INTEGER || decl->symbol_entry->data_type == DT_BOOLEAN)
            {
                fprintf(handle, "\t%s %s = 0;\n", "int", decl->symbol_entry->identifier);
            }
            else if (decl->symbol_entry->data_type == DT_CHAR_)
            {
                fprintf(handle, "\t%s %s = 0;\n", "char", decl->symbol_entry->identifier);
            }
        }
        else if (decl->expression != NULL)
        {
            if (decl->symbol_entry->data_type == DT_INTEGER || decl->symbol_entry->data_type == DT_BOOLEAN)
            {
                fprintf(handle, "\t%s %s = ", "int", decl->symbol_entry->identifier);
            }
            else if (decl->symbol_entry->data_type == DT_CHAR_)
            {
                fprintf(handle, "\t%s %s = ", "char", decl->symbol_entry->identifier);
            }
            ast_expression_printer(decl->expression, handle);
            fprintf(handle, "%s", ";\n");
        }
        
    }
}

void ast_array_declaration_printer(ast_node_array_declaration *decl, FILE *handle)
{
    if (decl != NULL && handle != NULL)
    {
        if (decl->initial_string == NULL)
        {
            if (decl->symbol_entry->data_type == DT_INT_ARR || decl->symbol_entry->data_type == DT_BOOL_ARR)
            {
                fprintf(handle, "\t%s %s[", "int", decl->symbol_entry->identifier);
                ast_expression_printer(decl->size, handle);
                fprintf(handle, "];\n");
            }
            else if (decl->symbol_entry->data_type == DT_CHAR_ARR)
            {
                fprintf(handle, "\t%s %s[", "char", decl->symbol_entry->identifier);
                ast_expression_printer(decl->size, handle);
                fprintf(handle, "];\n");
            }
        }
        else
        {
            fprintf(handle, "\t%s %s[", "char", decl->symbol_entry->identifier);
            ast_expression_printer(decl->size, handle);
            fprintf(handle, "] = %s;\n", decl->initial_string);
        }
    }
    else
    {
        printf("NULL!!\n");
    }
}

void ast_assignment_printer(ast_node_assignment *assg, FILE* handle)
{
    if (assg != NULL && handle != NULL)
    {
        fprintf(handle, "\t%s = ", assg->symbol_entry->identifier);
        ast_expression_printer(assg->expression, handle);
        fprintf(handle, "%s", ";\n");
    }
    
}

void ast_array_assignment_printer(ast_node_array_assignment *assign, FILE* handle)
{
    if (assign != NULL && handle != NULL)
    {
        fprintf(handle, "\t%s[", assign->symbol_entry->identifier);
        ast_expression_printer(assign->index, handle);
        fprintf(handle, "] = ");
        ast_expression_printer(assign->expression, handle);
        fprintf(handle, ";\n");
    }
}

void ast_array_access_printer(ast_node_array_access *access, FILE* handle)
{
    if (access != NULL && handle != NULL)
    {
        fprintf(handle, "\t%s[", access->symbol_entry->identifier);
        ast_expression_printer(access->index, handle);
        fprintf(handle, "]");
    }
}

void ast_expression_printer(ast_node_expression* node, FILE* handle)
{
    if (node != NULL && handle != NULL)
    {
        if (node->opt >= AST_OPR_BW_LFT && node->opt <= AST_OPR_LGL_OR)
        {
            fprintf(handle, "%s", "(");
            ast_expression_printer((ast_node_expression*)node->left, handle);
            switch (node->opt)
            {
                case AST_OPR_BW_LFT:
                    fprintf(handle, "%s", _OPR_LFT);
                    break;

                case AST_OPR_BW_RGT:
                    fprintf(handle, "%s", _OPR_RGT);
                    break;

                case AST_OPR_ADD:
                    fprintf(handle, "%s", _OPR_ADD);
                    break;     

                case AST_OPR_SUB:
                    fprintf(handle, "%s", _OPR_SUB);
                    break;     

                case AST_OPR_MUL:
                    fprintf(handle, "%s", _OPR_MUL);
                    break;     
    
                case AST_OPR_DIV:
                    fprintf(handle, "%s", _OPR_DIV);
                    break;     
                
                case AST_OPR_MOD:
                    fprintf(handle, "%s", _OPR_MOD);
                    break;

                case AST_OPR_GT:
                    fprintf(handle, "%s", _OPR_GT);
                    break;     
      
                case AST_OPR_LT:     
                    fprintf(handle, "%s", _OPR_LT);
                    break;     

                case AST_OPR_EQ:     
                    fprintf(handle, "%s", _OPR_EQ);
                    break;     

                case AST_OPR_NE:
                    fprintf(handle, "%s", _OPR_NE);
                    break;     
     
                case AST_OPR_GE: 
                    fprintf(handle, "%s", _OPR_GE);
                    break;     
    
                case AST_OPR_LE:
                    fprintf(handle, "%s", _OPR_LE);
                    break;     
     
                case AST_OPR_BW_NOT: 
                    fprintf(handle, "%s", _OPR_BW_NOT);
                    break;     
 
                case AST_OPR_BW_AND:  
                    fprintf(handle, "%s", _OPR_BW_AND);
                    break;     

                case AST_OPR_BW_OR:  
                    fprintf(handle, "%s", _OPR_BW_OR);
                    break;

                case AST_OPR_BW_XOR:  
                    fprintf(handle, "%s", _OPR_BW_XOR);
                    break;     
 
                case AST_OPR_LGL_NOT: 
                    fprintf(handle, "%s", _OPR_LGL_NOT);
                    break;     

                case AST_OPR_LGL_AND: 
                    fprintf(handle, "%s", _OPR_LGL_AND);
                    break;     

                case AST_OPR_LGL_OR:  
                    fprintf(handle, "%s", _OPR_LGL_OR);
                    break;     
            }
            ast_expression_printer((ast_node_expression*)node->right, handle);
            fprintf(handle, "%s", ")");
        }

        if (node->opt == AST_NODE_CONSTANT)
        {
            fprintf(handle, " %d ", node->value);
        }

        if (node->opt == AST_NODE_CONSTANT_CHAR)
        {
            fprintf(handle, " '%c' ", node->value);
        }

        if (node->opt == AST_NODE_VARIABLE)
        {
            if (((ast_node_variable*)node->left)->symbol_entry->is_constant == 1)
            {
                fprintf(handle, " %d ", ((ast_node_variable*)node->left)->symbol_entry->value);
            }
            else if (((ast_node_variable*)node->left)->symbol_entry->is_constant == 0)
            {
                fprintf(handle, " %s ", ((ast_node_variable*)node->left)->symbol_entry->identifier);
            }
            
        }

        if (node->opt == AST_NODE_ARRAY_ACCESS)
        {
            ast_array_access_printer((ast_node_array_access*)node->left, handle);
        }

        if (node->opt == AST_NODE_FUNC_CALL)
        {
            ast_function_call_printer((ast_node_function_call*)node->left, handle);
        }

        if (node->opt == AST_NODE_DIGITAL_READ_CALL)
        {
            ast_utility_function_call_printer((ast_node_utility_function_call*)node->left, handle);
        }

        if (node->opt == AST_NODE_READ_COUNTER_CALL)
        {
            ast_utility_function_call_printer((ast_node_utility_function_call*)node->left, handle);
        }

        if (node->opt == AST_NODE_RECV_RPMSG_CALL)
        {
            ast_utility_function_call_printer((ast_node_utility_function_call*)node->left, handle);
        }
    }
}

void ast_conditional_if_printer(ast_node_conditional_if *node, FILE* handle)
{
    if (node != NULL && handle != NULL)
    {
        fprintf(handle, "\t%s(", "if");
        ast_expression_printer(node->condition, handle);
        fprintf(handle, "%s", ")\n");
        ast_compound_statement_printer(node->body, handle, 0);

        if (node->else_if != NULL)
        {
            int i;
            ast_node_conditional_if *temp;
            vec_foreach(&node->else_if->else_if, temp, i)
            {
                fprintf(handle, "\t%s(", "else if");
                ast_expression_printer(temp->condition, handle);
                fprintf(handle, "%s", ")\n");
                ast_compound_statement_printer(temp->body, handle, 0);

            }
        }

        if (node->else_part != NULL)
        {
            fprintf(handle, "\t%s\n", "else");
            ast_compound_statement_printer(node->else_part, handle, 0);
        }
    }
}
void ast_loop_for_printer(ast_node_loop_for *node, FILE* handle)
{
    if (node != NULL && handle != NULL)
    {
        /*
        {
            for (int %identifier = %start; %identifier < %end; %identifier += %increment) %compound
            for (int %identifier = %start; %identifier > %end; %identifier += %increment) %compound
        }
        */

        fprintf(handle, "\nfor (int %s = ", node->init->symbol_entry->identifier);
        ast_expression_printer(node->range->start, handle);
        fprintf(handle, ";%s <", node->init->symbol_entry->identifier);
        ast_expression_printer(node->range->stop, handle);
        fprintf(handle, ";%s +=", node->init->symbol_entry->identifier);
        ast_expression_printer(node->range->increment, handle);
        fprintf(handle, ")\n");

        ast_compound_statement_printer(node->body, handle, 0);

        fprintf(handle, "for (int %s = ", node->init->symbol_entry->identifier);
        ast_expression_printer(node->range->start, handle);
        fprintf(handle, ";%s >", node->init->symbol_entry->identifier);
        ast_expression_printer(node->range->stop, handle);
        fprintf(handle, ";%s +=", node->init->symbol_entry->identifier);
        ast_expression_printer(node->range->increment, handle);
        fprintf(handle, ")\n");
        
        ast_compound_statement_printer(node->body, handle, 0);
    }
}

void ast_loop_while_printer(ast_node_loop_while *node, FILE* handle)
{
    if (node != NULL && handle != NULL)
    {
        fprintf(handle, "\t%s", "while(");
        ast_expression_printer(node->condition, handle);
        fprintf(handle, "%s", ")\n");
        ast_compound_statement_printer(node->body, handle, 0);
    }
}

void ast_function_call_printer(ast_node_function_call *fc, FILE* handle)
{
    if (fc != NULL && handle != NULL)
    {   
        fprintf(handle, "%s(", fc->symbol_entry->identifier);

        int iter;
        ast_node_expression *exp;
        if (fc->args != NULL)
        {
            vec_foreach(&fc->args->arguments, exp, iter)
            {
                ast_expression_printer(exp, handle);
                if (fc->args->arguments.length != iter+1)
                {
                    fprintf(handle, "%s", ",");
                }
            }
        }
        fprintf(handle, "%s", ")");
    }
}

void ast_utility_function_call_printer(ast_node_utility_function_call *ufc, FILE* handle)
{
    if (ufc != NULL && handle != NULL)
    {
        switch(ufc->node_type)
        {
            case AST_NODE_DIGITAL_READ_CALL:
                fprintf(handle, "%s(", "digital_read");
                ast_expression_printer(ufc->pin_number, handle);
                fprintf(handle, "%s", ")");

                if (config_pins(ufc->pin_number->value, IN, pru_id_) == -1)
                {
                    printf("(%d) : incorrect pin used in digital read call\n", ufc->pin_number->value);
                }
                break;

            case AST_NODE_DIGITAL_WRITE_CALL:
                fprintf(handle, "%s(", "digital_write");
                ast_expression_printer(ufc->pin_number, handle);
                fprintf(handle, "%s", ",");
                ast_expression_printer(ufc->value, handle);
                fprintf(handle, "%s", ")");

                if (config_pins(ufc->pin_number->value, OUT, pru_id_) == -1)
                {
                    printf("(%d) : incorrect pin used in digital write call\n", ufc->pin_number->value);
                }
                break;

            case AST_NODE_DELAY_CALL:
                fprintf(handle, "%s((", "__delay_cycles");
                ast_expression_printer(ufc->time_ms, handle);
                fprintf(handle, "%s", ")*200000)");
                break;

            case AST_NODE_PWM_CALL:
                fprintf(handle, "%s(", "pwm_write");
                ast_expression_printer(ufc->frequency, handle);
                fprintf(handle, "%s", ",");
                ast_expression_printer(ufc->duty_cycle, handle);
                fprintf(handle, "%s", ")");

                if (config_pins(ufc->pin_number->value, OUT, pru_id_) == -1)
                {
                    printf("(%d) : incorrect pin used in pwm call\n", ufc->pin_number->value);
                }
                break;
            
            case AST_NODE_START_COUNTER_CALL:
                fprintf(handle, "%s()", "start_counter");
                break;

            case AST_NODE_STOP_COUNTER_CALL:
                fprintf(handle, "%s()", "stop_counter");
                break;

            case AST_NODE_READ_COUNTER_CALL:
                fprintf(handle, "%s()", "read_counter");
                break;
            
            case AST_NODE_INIT_RPMSG_CALL:
                rpmsg_artifact_found = 1;

                fprintf(handle, "%s()", "init_rpmsg");
                break;

            case AST_NODE_RECV_RPMSG_CALL:
                rpmsg_artifact_found = 1;

                fprintf(handle, "%s()", "receive_rpmsg");
                break;

            case AST_NODE_SEND_INT_RPMSG_CALL:
                rpmsg_artifact_found = 1;

                fprintf(handle, "%s(", "send_int_rpmsg");
                ast_expression_printer(ufc->rpmsg_data, handle);
                fprintf(handle, "%s", ")");
                break;

            case AST_NODE_SEND_CHAR_RPMSG_CALL:
                rpmsg_artifact_found = 1;

                fprintf(handle, "%s(", "send_char_rpmsg");
                ast_expression_printer(ufc->rpmsg_data, handle);
                fprintf(handle, "%s", ")");
                break;

            case AST_NODE_SEND_BOOL_RPMSG_CALL:
                rpmsg_artifact_found = 1;

                fprintf(handle, "%s(", "send_bool_rpmsg");
                ast_expression_printer(ufc->rpmsg_data, handle);
                fprintf(handle, "%s", ")");
                break;

            // for arrays, we need to pass the length: sizeof(array) / sizeof(datatype)
            case AST_NODE_SEND_INTS_RPMSG_CALL:
                rpmsg_artifact_found = 1;

                fprintf(handle, "send_ints_rpmsg( %s, sizeof( %s ) / sizeof(int) )", ufc->symbol_entry->identifier, ufc->symbol_entry->identifier);
                break;

            case AST_NODE_SEND_CHARS_RPMSG_CALL:
                rpmsg_artifact_found = 1;

                fprintf(handle, "send_chars_rpmsg( %s, sizeof( %s ) / sizeof(char) )", ufc->symbol_entry->identifier, ufc->symbol_entry->identifier);
                break;

            case AST_NODE_SEND_BOOLS_RPMSG_CALL:
                rpmsg_artifact_found = 1;

                fprintf(handle, "send_bools_rpmsg( %s, sizeof( %s ) / sizeof(bool) )", ufc->symbol_entry->identifier, ufc->symbol_entry->identifier);
                break;
        }
    }
}

void ast_print_string_function_call_printer(ast_node_print_string_function_call *pfc, FILE* handle)
{
    if (pfc != NULL && handle != NULL)
    {
        fprintf(handle, "printf(\"%%s\", %s)", pfc->string);
        if (pfc->add_newline)
        {
            fprintf(handle, "; printf(\"\\n\")");
        }
    }
}

void ast_print_expression_function_call_printer(ast_node_print_expression_function_call *pfc, FILE *handle)
{
    if (pfc != NULL && handle != NULL)
    {
        fprintf(handle, "printf(\"%%d\", ");
        ast_expression_printer(pfc->expression, handle);
        fprintf(handle, ")");
        
        if (pfc->add_newline)
        {
            fprintf(handle, "; printf(\"\\n\")");
        }
    }
}

void ast_function_definition(ast_node_function_def *def, FILE* handle)
{
    if (def != NULL && handle != NULL)
    {   
        switch(def->symbol_entry->data_type)
        {
            case DT_INTEGER:
            case DT_BOOLEAN:
                fprintf(handle, "%s ", _DT_INT_);
                break;

            case DT_CHAR_:
                fprintf(handle, "%s ", _DT_CHAR_);
                break;

            case DT_VOID_:
                fprintf(handle, "%s ", _DT_VOID_);
                break;
        }
        fprintf(handle, "%s(", def->symbol_entry->identifier);

        if (def->params != NULL)
        {
            int i;
            ast_node_variable *temp;
            vec_foreach(&def->params->variable, temp, i)
            {
                switch(temp->symbol_entry->data_type)
                {
                    case DT_INTEGER:
                    case DT_BOOLEAN:
                        fprintf(handle, "%s ", _DT_INT_);
                        break;

                    case DT_CHAR_:
                        fprintf(handle, "%s ", _DT_CHAR_);
                        break;
                }
                fprintf(handle, "%s", temp->symbol_entry->identifier);
                if (def->params->variable.length != i+1)
                {
                    fprintf(handle, "%s", ", ");
                }
            }
        }
        fprintf(handle, "%s", ")\n");
        ast_compound_statement_printer(def->body, handle, 1);
        
        if (def->return_statment != NULL)
        {
            fprintf(handle, "\t%s ", "return");
            ast_expression_printer(def->return_statment, handle);
            fprintf(handle, "%s", ";\n");
        }
        fprintf(handle, "%s", "}\n");
    }
}

int code_printer(ast_node* ast, int pru_id, int test)
{
    FILE* handle = fopen("/tmp/temp.c", "w+");
    snprintf(pru_id_, 5, "pru%d", pru_id);

    if (handle == NULL)
    {
        fprintf(stderr, "Unable to create translated file\n");
        exit(0);
    }

    if (test == 0)
    {
        fprintf(handle, "%s", BEGIN);
        fprintf(handle, "%s", START_COUNTER);
        fprintf(handle, "%s", STOP_COUNTER);
        fprintf(handle, "%s", READ_COUNTER);
        fprintf(handle, "%s", DIGITAL_WRITE);
        fprintf(handle, "%s", DIGITAL_READ);
        fprintf(handle, "%s", RPMSG_DEFS);
    }
    else
    {
        fprintf(handle, "%s", TEST);
    }
    
    int i = 0;
    ast_node *temp;
    
    vec_foreach(&ast->child_nodes, temp, i)
    {
        if (temp->node_type == AST_NODE_FUNCTION_DEFS)
        {
            ast_function_definition((ast_node_function_def*)temp, handle);
        }
    }
    fprintf(handle, "%s", MAIN);
    temp = NULL;
    i = 0;

    vec_foreach(&ast->child_nodes, temp, i)
    {   
        switch(temp->node_type)
        {
            case AST_NODE_COMPOUND_STATEMENT:
                ast_compound_statement_printer(((ast_node_statements*)temp)->child_nodes.compound_statement, handle, 0);
                break;
            
            case AST_NODE_DECLARATION:
                ast_declaration_printer(((ast_node_statements*)temp)->child_nodes.declaration, handle);
                break;

            case AST_NODE_ARRAY_DECLARATION:
                ast_array_declaration_printer(((ast_node_statements*)temp)->child_nodes.array_declaration, handle);
                break;
            
            case AST_NODE_ASSIGNMENT:
                ast_assignment_printer(((ast_node_statements*)temp)->child_nodes.assignment, handle);
                break;
            
            case AST_NODE_ARRAY_ASSIGNMENT:
                ast_array_assignment_printer(((ast_node_statements*)temp)->child_nodes.array_assignment, handle);
                break;
            
            case AST_NODE_CONDITIONAL_IF:
                ast_conditional_if_printer(((ast_node_statements*)temp)->child_nodes.if_else, handle); 
                break;
        
            case AST_NODE_LOOP_FOR:
                ast_loop_for_printer(((ast_node_statements*)temp)->child_nodes.loop_for, handle);
                break;
            
            case AST_NODE_LOOP_WHILE:
                ast_loop_while_printer(((ast_node_statements*)temp)->child_nodes.loop_while, handle);
                break;
            
            case AST_NODE_FUNC_CALL:
                fprintf(handle, "%s", "\t");
                ast_function_call_printer(((ast_node_statements*)temp)->child_nodes.function_call, handle);
                fprintf(handle, "%s", ";\n");
                break;
            
            case AST_NODE_DIGITAL_READ_CALL:
            case AST_NODE_DIGITAL_WRITE_CALL:
            case AST_NODE_DELAY_CALL:
            case AST_NODE_PWM_CALL:
            case AST_NODE_START_COUNTER_CALL:
            case AST_NODE_STOP_COUNTER_CALL:
            case AST_NODE_READ_COUNTER_CALL:
            case AST_NODE_INIT_RPMSG_CALL:
            case AST_NODE_RECV_RPMSG_CALL:
            case AST_NODE_SEND_RPMSG_CALL:
                fprintf(handle, "%s", "\t");
                ast_utility_function_call_printer(((ast_node_statements*)temp)->child_nodes.utility_function_call, handle);
                fprintf(handle, "%s", ";\n");
                break;
            
            case AST_NODE_PRINT_STRING_FUNCTION_CALL:
                fprintf(handle, "%s", "\t");
                ast_print_string_function_call_printer(((ast_node_statements*)temp)->child_nodes.print_string_function_call, handle);
                fprintf(handle, "%s", ";\n");
                break;

            case AST_NODE_PRINT_EXP_FUNCTION_CALL:
                fprintf(handle, "%s", "\t");
                ast_print_expression_function_call_printer(((ast_node_statements*)temp)->child_nodes.print_expression_function_call, handle);
                fprintf(handle, "%s", ";\n");
                break;

        }
    }
    fprintf(handle, "%s", END);
    fclose(handle);

    return rpmsg_artifact_found;
}