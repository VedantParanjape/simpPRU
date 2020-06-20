#ifndef AST_H
#define AST_H

#include "vec/vec.h"
#include "symbol_table.h"

#define AST_NODE                     0
#define AST_NODE_TRANSLATIONAL_UNIT  1
#define AST_NODE_STATEMENTS          2
#define AST_NODE_FUNCTION_DEFS       3
#define AST_NODE_COMPOUND_STATEMENT  4
#define AST_NODE_EMPTY_STATEMENT     5
#define AST_NODE_DECLARATION         6
#define AST_NODE_ASSIGNMENT          7
#define AST_NODE_ARITHMETIC_EXP      8
#define AST_NODE_BOOLEAN_EXP         9
#define AST_NODE_LOGICAL_EXP         10
#define AST_NODE_CONDITIONAL_IF      11
#define AST_NODE_CONDITIONAL_ELSE_IF 12
#define AST_NODE_LOOP_FOR            13
#define AST_NODE_LOOP_WHILE          14
#define AST_NODE_LOOP_CONTROL        15
#define AST_NODE_FUNC_RETURN         16
#define AST_NODE_FUNC_CALL           17


#define AST_OPR_ADD        1 // + 
#define AST_OPR_SUB        2 // -
#define AST_OPR_MUL        3 // *
#define AST_OPR_DIV        4 // /

#define AST_OPR_GT         5 // > 
#define AST_OPR_LT         6 // <
#define AST_OPR_EQ         7 // =
#define AST_OPR_NE         8 // !=
#define AST_OPR_GE         9 // >=
#define AST_OPR_LE         10 // <=

#define AST_OPR_BW_NOT     11 // ~
#define AST_OPR_BW_AND     12 // &
#define AST_OPR_BW_OR      13 // |
#define AST_OPR_LGL_NOT    14 // not
#define AST_OPR_LGL_AND    15 // and
#define AST_OPR_LGL_OR     16 // or

#define AST_OPR_ASSIGNMENT 17 // :=

#define AST_CONST_INT 1 // INT CONSTANT
#define AST_CONST_BOOL 2 // BOOL CONSTANT
#define AST_IDENTIFIER 3 // IDENTIFIER

#define DT_INT  1 // DATA TYPE INT
#define DT_BOOL 2 // DATA TYPE BOOL
#define DT_VOID 3 // DATA TYPE VOID

typedef vec_t(struct ast_node*) ast_nodes;
typedef vec_t(struct ast_node_statments*) ast_nodes_statements;
typedef vec_t(struct ast_node_conditional_if*) ast_nodes_else_if;

typedef struct ast_node 
{
    int node_type;
    ast_nodes child_nodes;
}ast_node;

typedef struct ast_node_statements 
{
    int node_type;

    union child_nodes
    {
        ast_node_compound_statement *compount_statement;
        ast_node_declaration *declaration;
        ast_node_assignment *assignment;
        ast_node_conditional_if *if_else;
        ast_node_loop_for *loop_for;
        ast_node_loop_while *loop_while;
        ast_node_loop_control *loop_control;
    };
}ast_node_statements;

typedef struct ast_node_compound_statement 
{
    int node_type;
    
    ast_nodes_statements child_nodes;
}ast_node_compound_statement;

typedef struct ast_node_declaration
{
    int node_type;

    sym_ptr symbol_entry;
    ast_node_expression *expression;
}ast_node_declaration;

typedef struct ast_node_assignment
{
    int node_type;

    sym_ptr symbol_entry;
    ast_node_expression *expression;
}ast_node_assignment;

typedef struct ast_node_expression
{
    int node_type;
    int opt;

    ast_node *left;
    ast_node *right;
}ast_node_expression;

typedef struct ast_node_constant
{
    int node_type;
    int data_type;

    int value;
}ast_node_constant;

typedef struct ast_node_variable
{
    int node_type;
    int data_type;

    sym_ptr symbol_entry;
}ast_node_variable;

typedef struct ast_node_conditional_if
{
    int node_type;

    ast_node_expression *condition;
    ast_node_compound_statement *body;
    ast_nodes_else_if *else_if;
    ast_node_compound_statement *else_part;
}ast_node_conditional_if;

typedef struct ast_node_loop_for
{
    int node_type;

    ast_node_variable *init;
    ast_node_expression *start_condition;
    ast_node_expression *end_condition;
    ast_node_compound_statement *body;
}ast_node_loop_for;

typedef struct ast_node_loop_while
{
    int node_type;

    ast_node_expression *condition;
    ast_node_compound_statement *body;
}ast_node_loop_while;

typedef struct ast_node_loop_control
{
    int node_type;
}ast_node_loop_control;

// typedef struct ast_node_function_def
// {
//     int node_type;

//     sym_ptr symbol_entry;
//     ast_node_compound_statement *body;
// }ast_node_function_def;

#endif