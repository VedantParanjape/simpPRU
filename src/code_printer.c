#include "code_printer.h"

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
            
            case AST_NODE_ASSIGNMENT:
                ast_assignment_printer(((ast_node_statements*)temp)->child_nodes.assignment, handle);
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
            case AST_NODE_LOOP_CONTROL:
                if (((ast_node_statements*)temp)->child_nodes.loop_control->node_type == AST_NODE_LOOP_BREAK)
                {
                    fprintf(handle, "\t%s;\n", "break");
                }
                else if (((ast_node_statements*)temp)->child_nodes.loop_control->node_type == AST_NODE_LOOP_CONTINUE)
                {
                    fprintf(handle, "\t%s;\n", "continue");
                }
                break;
                
            case AST_NODE_DIGITAL_READ_CALL:
            case AST_NODE_DIGITAL_WRITE_CALL:
            case AST_NODE_DELAY_CALL:
            case AST_NODE_PWM_CALL:
            case AST_NODE_START_COUNTER_CALL:
            case AST_NODE_STOP_COUNTER_CALL:
            case AST_NODE_READ_COUNTER_CALL:
                fprintf(handle, "%s", "\t");
                ast_utility_function_call_printer(((ast_node_statements*)temp)->child_nodes.utility_function_call, handle);
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
                fprintf(handle, "\t%s %s;\n", "int", decl->symbol_entry->identifier);
            }
        }
        else if (decl->expression != NULL)
        {
            if (decl->symbol_entry->data_type == DT_INTEGER || decl->symbol_entry->data_type == DT_BOOLEAN)
            {
                fprintf(handle, "\t%s %s = ", "int", decl->symbol_entry->identifier);
            }
            ast_expression_printer(decl->expression, handle);
            fprintf(handle, "%s", ";\n");
        }
        
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

void ast_expression_printer(ast_node_expression* node, FILE* handle)
{
    if (node != NULL && handle != NULL)
    {
        if (node->opt >= 25 && node->opt <= 40)
        {
            ast_expression_printer((ast_node_expression*)node->left, handle);
            switch (node->opt)
            {
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
        }

        if (node->opt == AST_NODE_CONSTANT)
        {
            fprintf(handle, " %d ", node->value);
        }

        if (node->opt == AST_NODE_VARIABLE)
        {
            fprintf(handle, " %d ", node->value);
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
        fprintf(handle, "%s", "{\n");
        fprintf(handle, "\t%s %s;\n", "int", node->init->symbol_entry->identifier);
        fprintf(handle, "\t%s(%s =","for", node->init->symbol_entry->identifier);
        ast_expression_printer(node->start_condition, handle);
        fprintf(handle, ";%s <", node->init->symbol_entry->identifier);
        ast_expression_printer(node->end_condition, handle);
        fprintf(handle, ";%s++)\n", node->init->symbol_entry->identifier);
        ast_compound_statement_printer(node->body, handle, 0);
        fprintf(handle, "%s", "}\n");
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

void ast_function_call_printer(ast_node_function_call* fc, FILE* handle)
{
    if (fc != NULL && handle != NULL)
    {   
        fprintf(handle, "%s(", fc->symbol_entry->identifier);

        int iter;
        ast_node_expression *exp;
        vec_foreach(&fc->args->arguments, exp, iter)
        {
            ast_expression_printer(exp, handle);
            if (fc->args->arguments.length != iter+1)
            {
                fprintf(handle, "%s", ",");
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
                break;

            case AST_NODE_DIGITAL_WRITE_CALL:
                fprintf(handle, "%s(", "digital_write");
                ast_expression_printer(ufc->pin_number, handle);
                fprintf(handle, "%s", ",");
                ast_expression_printer(ufc->value, handle);
                fprintf(handle, "%s", ")");
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

void code_printer(ast_node* ast)
{
    FILE* handle = fopen("../generated_code/temp.c", "w");

    int i = 0;
    ast_node *temp;

    fprintf(handle, "%s", BEGIN);       
    vec_foreach(&ast->child_nodes, temp, i)
    {
        if (temp->node_type == AST_NODE_FUNCTION_DEFS)
        {
            ast_function_definition((ast_node_function_def*)temp, handle);
        }
    }
    temp = NULL;
    i = 0;

    fprintf(handle, "%s", START_COUNTER);
    fprintf(handle, "%s", STOP_COUNTER);
    fprintf(handle, "%s", READ_COUNTER);
    fprintf(handle, "%s", DIGITAL_WRITE);
    fprintf(handle, "%s", DIGITAL_READ);

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
            
            case AST_NODE_ASSIGNMENT:
                ast_assignment_printer(((ast_node_statements*)temp)->child_nodes.assignment, handle);
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
                fprintf(handle, "%s", "\t");
                ast_utility_function_call_printer(((ast_node_statements*)temp)->child_nodes.utility_function_call, handle);
                fprintf(handle, "%s", ";\n");
                break;
        }
    }
    fprintf(handle, "%s", END);
    fclose(handle);
}