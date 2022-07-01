#include "ast.h"

ast_node *create_translation_unit()
{
    ast_node *tu = (ast_node*)malloc(sizeof(ast_node));

    tu->node_type = AST_NODE_TRANSLATIONAL_UNIT;
    vec_init(&tu->child_nodes);

    return tu;
}

ast_node *add_program_unit(ast_node *parent, ast_node *child)
{
    vec_push(&parent->child_nodes, child);

    return parent;
}

ast_node_statements *create_statement_node(int node_type, void *child)
{
    ast_node_statements *stmt = (ast_node_statements*)malloc(sizeof(ast_node_statements));

    stmt->node_type = node_type;
    switch (node_type)
    {
        case AST_NODE_COMPOUND_STATEMENT:
            stmt->child_nodes.compound_statement = child;
            break;

        case AST_NODE_DECLARATION:
            stmt->child_nodes.declaration = child;
            break;    
        
        case AST_NODE_ARRAY_DECLARATION:
            stmt->child_nodes.array_declaration = child;
            break;
        
        case AST_NODE_ASSIGNMENT:
            stmt->child_nodes.assignment = child;
            break;

        case AST_NODE_UNARY_INC:
            stmt->child_nodes.unary_increment= child;
            break;            

        case AST_NODE_ARRAY_ASSIGNMENT:
            stmt->child_nodes.array_assignment = child;
            break;
        
        case AST_NODE_CONDITIONAL_IF:
            stmt->child_nodes.if_else = child;
            break;
        
        case AST_NODE_LOOP_FOR:
            stmt->child_nodes.loop_for = child;
            break;
        
        case AST_NODE_LOOP_WHILE: 
            stmt->child_nodes.loop_while = child;
            break;
        
        case AST_NODE_LOOP_BREAK:
        case AST_NODE_LOOP_CONTINUE:
            stmt->child_nodes.loop_control = child;
            break;
        
        case AST_NODE_FUNC_RETURN:
            stmt->child_nodes.return_statement = child;
            break;
            
        case AST_NODE_FUNC_CALL:
            stmt->child_nodes.function_call = child;
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
            stmt->child_nodes.utility_function_call = child;
            break;
        
        case AST_NODE_PRINT_STRING_FUNCTION_CALL:
            stmt->child_nodes.print_string_function_call = child;
            break;
        case AST_NODE_PRINT_EXP_FUNCTION_CALL:
            stmt->child_nodes.print_expression_function_call = child;
            break;
    }

    return stmt;
}

ast_node_compound_statement *create_compound_statement_node()
{
    ast_node_compound_statement *cmpd_stmt = (ast_node_compound_statement*)malloc(sizeof(ast_node_compound_statement));

    cmpd_stmt->node_type = AST_NODE_COMPOUND_STATEMENT;
    vec_init(&cmpd_stmt->child_nodes);

    return cmpd_stmt;
}

ast_node_compound_statement *add_compound_statement_node(ast_node_compound_statement *parent, ast_node_statements *child)
{
    vec_push(&parent->child_nodes, child);

    return parent;
}

ast_node_declaration *create_declaration_node(sym_ptr symbol, ast_node_expression *exp)
{
    ast_node_declaration *decl = (ast_node_declaration*)malloc(sizeof(ast_node_declaration));

    decl->node_type = AST_NODE_DECLARATION;
    decl->expression = exp;
    decl->symbol_entry = symbol;

    return decl;
}

ast_node_array_declaration *create_array_declaration_node(sym_ptr symbol, ast_node_expression *size, char *initial_string)
{
    ast_node_array_declaration *decl = (ast_node_array_declaration*)malloc(sizeof(ast_node_array_declaration));

    decl->node_type = AST_NODE_ARRAY_DECLARATION;
    decl->initial_string = initial_string;
    decl->size = size;
    decl->symbol_entry = symbol;

    return decl;
}

ast_node_assignment *create_assignment_node(sym_ptr symbol, ast_node_expression *exp)
{
    ast_node_assignment *assgn = (ast_node_assignment*)malloc(sizeof(ast_node_assignment));

    assgn->node_type = AST_NODE_ASSIGNMENT;
    assgn->expression = exp;
    assgn->symbol_entry = symbol;

    return assgn;
}

ast_node_unary_increment *create_unary_increment_node(sym_ptr symbol)
{
    ast_node_unary_increment *assgn = (ast_node_unary_increment*)malloc(sizeof(ast_node_unary_increment));

    assgn->node_type = AST_NODE_UNARY_INC;
    assgn->symbol_entry = symbol;

    return assgn;
}

ast_node_array_assignment *create_array_assignment_node(sym_ptr symbol, ast_node_expression *index, ast_node_expression *exp)
{
    ast_node_array_assignment *assign = (ast_node_array_assignment*)malloc(sizeof(ast_node_array_assignment));

    assign->node_type = AST_NODE_ARRAY_ASSIGNMENT;
    assign->expression = exp;
    assign->index = index;
    assign->symbol_entry = symbol;

    return assign;
}

ast_node_array_access *create_array_access_node(sym_ptr symbol, ast_node_expression *index)
{
    ast_node_array_access *access = (ast_node_array_access*)malloc(sizeof(ast_node_array_access));

    access->node_type = AST_NODE_ARRAY_ACCESS;
    access->index = index;
    access->symbol_entry = symbol;

    return access;
}

ast_node_expression *create_expression_node(int node_type, int opt, int value, ast_node *left, ast_node *right)
{
    ast_node_expression *exp = (ast_node_expression*)malloc(sizeof(ast_node_expression));

    exp->node_type = node_type;
    exp->opt = opt;
    exp->value = value;
    exp->left = left;
    exp->right = right;

    return exp;
}

ast_node_range_expression *create_range_expression_node(ast_node_expression *start, ast_node_expression *stop, ast_node_expression *increment)
{
    ast_node_range_expression *exp = (ast_node_range_expression*)malloc(sizeof(ast_node_range_expression));

    exp->node_type = AST_NODE_RANGE_EXP;
    if (!start)
    {
        start = create_expression_node(AST_NODE_ARITHMETIC_EXP, AST_NODE_CONSTANT, 0, NULL, NULL);
    }
    exp->start = start;
    exp->stop = stop;
    if (!increment)
    {
        increment = create_expression_node(AST_NODE_ARITHMETIC_EXP, AST_NODE_CONSTANT, 1, NULL, NULL);
    }
    exp->increment = increment;

    return exp;
}

ast_node_constant *create_constant_node(int data_type, int value)
{
    ast_node_constant *cnst = (ast_node_constant*)malloc(sizeof(ast_node_constant)); 

    cnst->node_type = AST_NODE_CONSTANT;
    cnst->data_type = data_type;
    cnst->value = value;

    return cnst;
}

ast_node_variable *create_variable_node(int data_type, sym_ptr symbol)
{
    ast_node_variable *var = (ast_node_variable*)malloc(sizeof(ast_node_variable));

    var->node_type = AST_NODE_VARIABLE;
    var->data_type = data_type;
    var->symbol_entry = symbol;

    return var;
}

ast_node_conditional_if *create_conditional_if_node(ast_node_expression *condition, ast_node_compound_statement *body, ast_node_conditional_else_if *else_if, ast_node_compound_statement *else_node)
{
    ast_node_conditional_if *cond_if = (ast_node_conditional_if*)malloc(sizeof(ast_node_conditional_if));

    cond_if->node_type = AST_NODE_CONDITIONAL_IF;
    cond_if->condition = condition;
    cond_if->body = body;
    cond_if->else_if = else_if;
    cond_if->else_part = else_node;

    return cond_if;
}

ast_node_conditional_else_if *create_else_if_node()
{
    ast_node_conditional_else_if *cond_else_if = (ast_node_conditional_else_if*)malloc(sizeof(ast_node_conditional_else_if));

    cond_else_if->node_type = AST_NODE_CONDITIONAL_ELSE_IF;
    vec_init(&cond_else_if->else_if);

    return cond_else_if;
}

ast_node_conditional_else_if *add_else_if_node(ast_node_conditional_else_if *parent, ast_node_expression *condition, ast_node_compound_statement *body)
{
    ast_node_conditional_if *temp = (ast_node_conditional_if*)malloc(sizeof(ast_node_conditional_if));

    temp->node_type = AST_NODE_CONDITIONAL_ELSE_IF;
    temp->condition = condition;
    temp->body = body;
    temp->else_if = NULL;
    temp->else_part = NULL;

    vec_push(&parent->else_if, temp);

    return parent;
}

ast_node_loop_for *create_loop_for_node(ast_node_variable *init, ast_node_range_expression *range, ast_node_compound_statement *body)
{
    ast_node_loop_for *loop_for = (ast_node_loop_for*)malloc(sizeof(ast_node_loop_for));

    loop_for->node_type = AST_NODE_LOOP_FOR;
    loop_for->init = init;
    loop_for->range = range;
    loop_for->body = body;

    return loop_for;
}

ast_node_loop_while *create_loop_while_node(ast_node_expression *condition, ast_node_compound_statement *body)
{
    ast_node_loop_while *loop_while = (ast_node_loop_while*)malloc(sizeof(ast_node_loop_while));

    loop_while->node_type = AST_NODE_LOOP_FOR;
    loop_while->condition = condition;
    loop_while->body = body;

    return loop_while;
}

ast_node_loop_control *create_loop_control_node(int node_type)
{
    ast_node_loop_control *loop_control = (ast_node_loop_control*)malloc(sizeof(ast_node_loop_control));

    loop_control->node_type = node_type;

    return loop_control;
}

ast_node_function_def *create_function_def_node(sym_ptr symbol_entry, ast_node_param *params, ast_node_compound_statement *body, ast_node_expression *return_stmt)
{
    ast_node_function_def *function_def = (ast_node_function_def*)malloc(sizeof(ast_node_function_def));

    function_def->node_type = AST_NODE_FUNCTION_DEFS;
    function_def->symbol_entry = symbol_entry;
    function_def->params = params;
    function_def->body = body;
    function_def->return_statment = return_stmt;

    return function_def;
}

ast_node_param *create_parameter_node()
{
    ast_node_param *params = (ast_node_param*)malloc(sizeof(ast_node_param));

    params->node_type = AST_NODE_FUNC_PARAM;
    vec_init(&params->variable);

    return params;
}

ast_node_param *add_parameter_node(ast_node_param *parent, ast_node_variable *var)
{
    vec_push(&parent->variable, var);

    return parent;
}

ast_node_function_call *create_function_call_node(sym_ptr symbol, ast_node_arguments *arguments)
{
    ast_node_function_call *function_call = (ast_node_function_call*)malloc(sizeof(ast_node_function_call));

    function_call->node_type = AST_NODE_FUNC_CALL;
    function_call->symbol_entry = symbol;
    function_call->args = arguments;

    return function_call;
}

ast_node_arguments *create_argument_node()
{
    ast_node_arguments *arguments = (ast_node_arguments*)malloc(sizeof(ast_node_arguments));

    arguments->node_type = AST_NODE_FUNC_ARGS;
    vec_init(&arguments->arguments);

    return arguments;
}

ast_node_arguments *add_argument_node(ast_node_arguments *parent, ast_node_expression *argument)
{
    vec_push(&parent->arguments, argument);

    return parent;
}

ast_node_utility_function_call *create_digital_read_call_node(ast_node_expression *pin_number)
{
    ast_node_utility_function_call *utility_function_call = (ast_node_utility_function_call*)malloc(sizeof(ast_node_utility_function_call));

    utility_function_call->node_type = AST_NODE_DIGITAL_READ_CALL;
    utility_function_call->pin_number = pin_number;
    utility_function_call->value = NULL;
    utility_function_call->time_ms = NULL;
    utility_function_call->frequency = NULL;
    utility_function_call->duty_cycle = NULL;
    utility_function_call->rpmsg_data = NULL;
    utility_function_call->symbol_entry = NULL;

    return utility_function_call;
}

ast_node_utility_function_call *create_digital_write_call_node(ast_node_expression *pin_number, ast_node_expression *value)
{
    ast_node_utility_function_call *utility_function_call = (ast_node_utility_function_call*)malloc(sizeof(ast_node_utility_function_call));

    utility_function_call->node_type = AST_NODE_DIGITAL_WRITE_CALL;
    utility_function_call->pin_number = pin_number;
    utility_function_call->value = value;
    utility_function_call->time_ms = NULL;
    utility_function_call->frequency = NULL;
    utility_function_call->duty_cycle = NULL;
    utility_function_call->rpmsg_data = NULL;
    utility_function_call->symbol_entry = NULL;
    
    return utility_function_call;
}

ast_node_utility_function_call *create_delay_call_node(ast_node_expression *time_ms)
{
    ast_node_utility_function_call *utility_function_call = (ast_node_utility_function_call*)malloc(sizeof(ast_node_utility_function_call));

    utility_function_call->node_type = AST_NODE_DELAY_CALL;
    utility_function_call->pin_number = NULL;
    utility_function_call->value = NULL;
    utility_function_call->time_ms = time_ms;
    utility_function_call->frequency = NULL;
    utility_function_call->duty_cycle = NULL;
    utility_function_call->rpmsg_data = NULL;
    utility_function_call->symbol_entry = NULL;

    return utility_function_call;
}

ast_node_utility_function_call *create_pwm_call_node(ast_node_expression *freq, ast_node_expression *duty)
{
    ast_node_utility_function_call *utility_function_call = (ast_node_utility_function_call*)malloc(sizeof(ast_node_utility_function_call));

    utility_function_call->node_type = AST_NODE_PWM_CALL;
    utility_function_call->pin_number = NULL;
    utility_function_call->value = NULL;
    utility_function_call->time_ms = NULL;
    utility_function_call->frequency = freq;
    utility_function_call->duty_cycle = duty;
    utility_function_call->rpmsg_data = NULL;
    utility_function_call->symbol_entry = NULL;

    return utility_function_call;
}

ast_node_utility_function_call *create_start_counter_call_node()
{
    ast_node_utility_function_call *utility_function_call = (ast_node_utility_function_call*)malloc(sizeof(ast_node_utility_function_call));

    utility_function_call->node_type = AST_NODE_START_COUNTER_CALL;
    utility_function_call->pin_number = NULL;
    utility_function_call->value = NULL;
    utility_function_call->time_ms = NULL;
    utility_function_call->frequency = NULL;
    utility_function_call->duty_cycle = NULL;
    utility_function_call->rpmsg_data = NULL;
    utility_function_call->symbol_entry = NULL;

    return utility_function_call;
}

ast_node_utility_function_call *create_stop_counter_call_node()
{
    ast_node_utility_function_call *utility_function_call = (ast_node_utility_function_call*)malloc(sizeof(ast_node_utility_function_call));

    utility_function_call->node_type = AST_NODE_STOP_COUNTER_CALL;
    utility_function_call->pin_number = NULL;
    utility_function_call->value = NULL;
    utility_function_call->time_ms = NULL;
    utility_function_call->frequency = NULL;
    utility_function_call->duty_cycle = NULL;
    utility_function_call->rpmsg_data = NULL;
    utility_function_call->symbol_entry = NULL;

    return utility_function_call;
}

ast_node_utility_function_call *create_read_counter_call_node()
{
    ast_node_utility_function_call *utility_function_call = (ast_node_utility_function_call*)malloc(sizeof(ast_node_utility_function_call));

    utility_function_call->node_type = AST_NODE_READ_COUNTER_CALL;
    utility_function_call->pin_number = NULL;
    utility_function_call->value = NULL;
    utility_function_call->time_ms = NULL;
    utility_function_call->frequency = NULL;
    utility_function_call->duty_cycle = NULL;
    utility_function_call->rpmsg_data = NULL;
    utility_function_call->symbol_entry = NULL;

    return utility_function_call;
}

ast_node_utility_function_call *create_init_rpmsg_call_node()
{
    ast_node_utility_function_call *utility_function_call = (ast_node_utility_function_call*)malloc(sizeof(ast_node_utility_function_call));

    utility_function_call->node_type = AST_NODE_INIT_RPMSG_CALL;
    utility_function_call->pin_number = NULL;
    utility_function_call->value = NULL;
    utility_function_call->time_ms = NULL;
    utility_function_call->frequency = NULL;
    utility_function_call->duty_cycle = NULL;
    utility_function_call->rpmsg_data = NULL;
    utility_function_call->symbol_entry = NULL;

    return utility_function_call;
}

ast_node_utility_function_call *create_recv_rpmsg_call_node()
{
    ast_node_utility_function_call *utility_function_call = (ast_node_utility_function_call*)malloc(sizeof(ast_node_utility_function_call));

    utility_function_call->node_type = AST_NODE_RECV_RPMSG_CALL;
    utility_function_call->pin_number = NULL;
    utility_function_call->value = NULL;
    utility_function_call->time_ms = NULL;
    utility_function_call->frequency = NULL;
    utility_function_call->duty_cycle = NULL;
    utility_function_call->rpmsg_data = NULL;
    utility_function_call->symbol_entry = NULL;

    return utility_function_call;
}

ast_node_utility_function_call *create_send_rpmsg_call_node(int node_type, ast_node_expression *rpmsg_data)
{
    ast_node_utility_function_call *utility_function_call = (ast_node_utility_function_call*)malloc(sizeof(ast_node_utility_function_call));

    utility_function_call->node_type = node_type;
    utility_function_call->pin_number = NULL;
    utility_function_call->value = NULL;
    utility_function_call->time_ms = NULL;
    utility_function_call->frequency = NULL;
    utility_function_call->duty_cycle = NULL;
    utility_function_call->rpmsg_data = rpmsg_data;
    utility_function_call->symbol_entry = NULL;

    return utility_function_call;
}

ast_node_utility_function_call *create_send_rpmsg_call_node_arrays(int node_type, sym_ptr array_identifier)
{
    ast_node_utility_function_call *utility_function_call = (ast_node_utility_function_call*)malloc(sizeof(ast_node_utility_function_call));

    utility_function_call->node_type = node_type;
    utility_function_call->pin_number = NULL;
    utility_function_call->value = NULL;
    utility_function_call->time_ms = NULL;
    utility_function_call->frequency = NULL;
    utility_function_call->duty_cycle = NULL;
    utility_function_call->rpmsg_data = NULL;
    utility_function_call->symbol_entry = array_identifier;

    return utility_function_call;
}

ast_node_print_string_function_call *create_print_string_function_call_node(char *string, int add_newline)
{
    ast_node_print_string_function_call *print_function_call = (ast_node_print_string_function_call*)malloc(sizeof(ast_node_print_string_function_call));

    print_function_call->node_type = AST_NODE_PRINT_STRING_FUNCTION_CALL;
    print_function_call->add_newline = add_newline;
    print_function_call->string = string;

    return print_function_call;
}

ast_node_print_expression_function_call *create_print_expression_function_call_node(ast_node_expression *expression, int add_newline)
{
    ast_node_print_expression_function_call *print_function_call = (ast_node_print_expression_function_call*)malloc(sizeof(ast_node_print_expression_function_call));

    print_function_call->node_type = AST_NODE_PRINT_EXP_FUNCTION_CALL;
    print_function_call->add_newline = add_newline;
    
    print_function_call->expression = expression;

    return print_function_call;
}

void ast_node_dump(ast_node* ast)
{
    ast_node_type(ast->node_type);

    int i = 0;
    ast_node *temp;
    
    vec_foreach(&ast->child_nodes, temp, i)
    {
        ast_node_type(temp->node_type);   
    }
}

void ast_node_type(int node_type)
{
    switch (node_type)
    {
        case AST_NODE:
            printf("ast node");
            break;

        case AST_NODE_TRANSLATIONAL_UNIT:
            printf("ast translational unit");
            break;

        case AST_NODE_STATEMENTS:
            printf("ast statements");
            break;

        case AST_NODE_FUNCTION_DEFS:
            printf("ast function definition");
            break;

        case AST_NODE_COMPOUND_STATEMENT:
            printf("ast compound statement");
            break;

        case AST_NODE_EMPTY_STATEMENT:
            printf("ast empty statement");
            break;

        case AST_NODE_DECLARATION:
            printf("ast declaration");
            break;

        case AST_NODE_ARRAY_DECLARATION:
            printf("ast array declaration");
            break;

        case AST_NODE_ASSIGNMENT:
            printf("ast assignment");
            break;

        case AST_NODE_UNARY_INC:
            printf("ast unary increment");
            break;                        

        case AST_NODE_ARRAY_ASSIGNMENT:
            printf("ast array assignment");
            break;

        case AST_NODE_ARITHMETIC_EXP:
            printf("ast arithmetic expression");
            break;

        case AST_NODE_BOOLEAN_EXP:
            printf("ast boolean expression");
            break;

        case AST_NODE_RELATIONAL_EXP:
            printf("ast relational expression");
            break;

        case AST_NODE_LOGICAL_EXP:
            printf("ast logical expression");
            break;

        case AST_NODE_RANGE_EXP:
            printf("ast node range expression");
            break;

        case AST_NODE_CONSTANT:
            printf("ast constant");
            break;

        case AST_NODE_CONSTANT_CHAR:
            printf("ast_constant_char");
            break;

        case AST_NODE_VARIABLE:
            printf("ast variable");
            break;

        case AST_NODE_CONDITIONAL_IF:
            printf("ast if statement");
            break;

        case AST_NODE_CONDITIONAL_ELSE_IF:
            printf("ast elseif statement");
            break;

        case AST_NODE_LOOP_FOR:
            printf("ast for statement");
            break;

        case AST_NODE_LOOP_WHILE:
            printf("ast while statement");
            break;

        case AST_NODE_LOOP_CONTROL:
            printf("ast control statement");
            break;

        case AST_NODE_LOOP_BREAK:
            printf("ast break statement");
            break;

        case AST_NODE_LOOP_CONTINUE:
            printf("ast continue statement");
            break;

        case AST_NODE_FUNC_CALL:
            printf("ast function call");
            break;
        
        case AST_NODE_FUNC_PARAM:
            printf("ast parameters");
            break;
        
        case AST_NODE_FUNC_ARGS:
            printf("ast arguments");
            break;

        case AST_NODE_DIGITAL_READ_CALL:
            printf("ast digital read");
            break;

        case AST_NODE_DIGITAL_WRITE_CALL:
            printf("ast digital write");
            break;

        case AST_NODE_DELAY_CALL:
            printf("ast delay");
            break;

        case AST_NODE_PWM_CALL:
            printf("pwm call");
            break;

        case AST_NODE_START_COUNTER_CALL:
            printf("start counter call");
            break;

        case AST_NODE_STOP_COUNTER_CALL:
            printf("stop counter call");
            break;

        case AST_NODE_READ_COUNTER_CALL:
            printf("read counter call");
            break;

        case AST_NODE_INIT_RPMSG_CALL:
            printf("init rpmsg call");
            break;

        case AST_NODE_RECV_RPMSG_CALL:
            printf("recv rpmsg call");
            break;

        case AST_NODE_SEND_RPMSG_CALL:
        case AST_NODE_SEND_INT_RPMSG_CALL:
        case AST_NODE_SEND_CHAR_RPMSG_CALL:
        case AST_NODE_SEND_BOOL_RPMSG_CALL:
        case AST_NODE_SEND_INTS_RPMSG_CALL:
        case AST_NODE_SEND_CHARS_RPMSG_CALL:
        case AST_NODE_SEND_BOOLS_RPMSG_CALL:
            printf("send rpmsg call");
            break;

        case AST_NODE_PRINT_STRING_FUNCTION_CALL:
            printf("print_string function call");
            break;

        case AST_NODE_PRINT_EXP_FUNCTION_CALL:
            printf("print_expression function call");
            break;

        default:
            printf("ast invalid");
            break;
    }
    printf("\n");
}