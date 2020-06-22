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
            stmt->child_nodes.compount_statement = child;
            break;

        case AST_NODE_DECLARATION:
            stmt->child_nodes.declaration = child;
            break;    
        
        case AST_NODE_ASSIGNMENT:
            stmt->child_nodes.assignment = child;
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
        
        case AST_NODE_LOOP_CONTROL:
            stmt->child_nodes.loop_control = child;
            break;
        
        case AST_NODE_FUNC_CALL:
            stmt->child_nodes.function_call = child;
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

ast_node_assignment *create_assignment_node(sym_ptr symbol, ast_node_expression *exp)
{
    ast_node_assignment *assgn = (ast_node_assignment*)malloc(sizeof(ast_node_assignment));

    assgn->node_type = AST_NODE_ASSIGNMENT;
    assgn->expression = exp;
    assgn->symbol_entry = symbol;

    return assgn;
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

ast_node_loop_for *create_loop_for_node(ast_node_variable *init, ast_node_expression *start, ast_node_expression *end, ast_node_compound_statement *body)
{
    ast_node_loop_for *loop_for = (ast_node_loop_for*)malloc(sizeof(ast_node_loop_for));

    loop_for->node_type = AST_NODE_LOOP_FOR;
    loop_for->init = init;
    loop_for->start_condition = start;
    loop_for->end_condition = end;
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