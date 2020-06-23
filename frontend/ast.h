#ifndef AST_H
#define AST_H

#include "vec/vec.h"
#include "symbol_table.h"
#include <stdio.h>

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
#define AST_NODE_RELATIONAL_EXP      10
#define AST_NODE_LOGICAL_EXP         11
#define AST_NODE_CONSTANT            12
#define AST_NODE_VARIABLE            13
#define AST_NODE_CONDITIONAL_IF      14
#define AST_NODE_CONDITIONAL_ELSE_IF 15
#define AST_NODE_LOOP_FOR            16
#define AST_NODE_LOOP_WHILE          17
#define AST_NODE_LOOP_CONTROL        18
#define AST_NODE_LOOP_BREAK          19
#define AST_NODE_LOOP_CONTINUE       20
#define AST_NODE_FUNC_CALL           21
#define AST_NODE_FUNC_PARAM          22
#define AST_NODE_FUNC_RETURN         23
#define AST_NODE_FUNC_ARGS           24

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

#define AST_DT_INT  1 // DATA TYPE INT
#define AST_DT_BOOL 2 // DATA TYPE BOOL
#define AST_DT_VOID 3 // DATA TYPE VOID

typedef vec_t(struct ast_node*) ast_nodes;
typedef vec_t(struct ast_node_statements*) ast_nodes_statements;
typedef vec_t(struct ast_node_conditional_if*) ast_nodes_else_if;
typedef vec_t(struct ast_node_variable*) ast_nodes_variables;
typedef vec_t(struct ast_node_expression*) ast_nodes_expressions;

struct ast_node;
struct ast_node_statements;
struct ast_node_compound_statement;
struct ast_node_declaration;
struct ast_node_assignment;
struct ast_node_expression;
struct ast_node_constant;
struct ast_node_variable;
struct ast_node_conditional_if;
struct ast_node_conditional_else_if;
struct ast_node_loop_for;
struct ast_node_loop_while;
struct ast_node_loop_control;
struct ast_node_function_def;
struct ast_node_param;
struct ast_node_function_call;
struct ast_node_arguments;

typedef struct ast_node ast_node;
typedef struct ast_node_statements ast_node_statements;
typedef struct ast_node_compound_statement ast_node_compound_statement;
typedef struct ast_node_declaration ast_node_declaration;
typedef struct ast_node_assignment ast_node_assignment;
typedef struct ast_node_expression ast_node_expression;
typedef struct ast_node_constant ast_node_constant;
typedef struct ast_node_variable ast_node_variable;
typedef struct ast_node_conditional_if ast_node_conditional_if;
typedef struct ast_node_conditional_else_if ast_node_conditional_else_if;
typedef struct ast_node_loop_for ast_node_loop_for;
typedef struct ast_node_loop_while ast_node_loop_while;
typedef struct ast_node_loop_control ast_node_loop_control;
typedef struct ast_node_function_def ast_node_function_def;
typedef struct ast_node_param ast_node_param;
typedef struct ast_node_function_call ast_node_function_call;
typedef struct ast_node_arguments ast_node_arguments;


struct ast_node 
{
    int node_type;
    ast_nodes child_nodes;
};

 struct ast_node_statements 
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
        ast_node_expression *return_statement;
        ast_node_function_call *function_call;
    }child_nodes;
};

 struct ast_node_compound_statement
{
    int node_type;
    
    ast_nodes_statements child_nodes;
};

 struct ast_node_declaration
{
    int node_type;

    sym_ptr symbol_entry;
    ast_node_expression *expression;
};

 struct ast_node_assignment
{
    int node_type;

    sym_ptr symbol_entry;
    ast_node_expression *expression;
};

 struct ast_node_expression
{
    int node_type;
    int opt;
    int value;

    ast_node *left;
    ast_node *right;
};

 struct ast_node_constant
{
    int node_type;
    int data_type;

    int value;
};

 struct ast_node_variable
{
    int node_type;
    int data_type;

    sym_ptr symbol_entry;
};

 struct ast_node_conditional_if
{
    int node_type;

    ast_node_expression *condition;
    ast_node_compound_statement *body;
    ast_node_conditional_else_if *else_if;
    ast_node_compound_statement *else_part;
};

 struct ast_node_conditional_else_if
{
    int node_type;

    ast_nodes_else_if else_if;
};

 struct ast_node_loop_for
{
    int node_type;

    ast_node_variable *init;
    ast_node_expression *start_condition;
    ast_node_expression *end_condition;
    ast_node_compound_statement *body;
};

 struct ast_node_loop_while
{
    int node_type;

    ast_node_expression *condition;
    ast_node_compound_statement *body;
};

 struct ast_node_loop_control
{
    int node_type;
};

 struct ast_node_function_def
{
    int node_type;

    sym_ptr symbol_entry;
    ast_node_param *params;
    ast_node_compound_statement *body;
    ast_node_expression *return_statment;
};

 struct ast_node_param 
{
    int node_type;

    ast_nodes_variables variable;
};

 struct ast_node_function_call
{
    int node_type;

    sym_ptr symbol_entry;
    ast_node_arguments *args;
};

struct ast_node_arguments
{
    int node_type;

    ast_nodes_expressions arguments;
};

ast_node *create_translation_unit();
ast_node *add_program_unit(ast_node *parent, ast_node *child);
ast_node_statements *create_statement_node(int node_type, void *child);
ast_node_compound_statement *create_compound_statement_node();
ast_node_compound_statement *add_compound_statement_node(ast_node_compound_statement *parent, ast_node_statements *child);
ast_node_declaration *create_declaration_node(sym_ptr symbol, ast_node_expression *exp);
ast_node_assignment *create_assignment_node(sym_ptr symbol, ast_node_expression *exp);
ast_node_expression *create_expression_node(int node_type, int opt, int value, ast_node *left, ast_node *right);
ast_node_constant *create_constant_node(int data_type, int value);
ast_node_variable *create_variable_node(int data_type, sym_ptr symbol);
ast_node_conditional_if *create_conditional_if_node(ast_node_expression *condition, ast_node_compound_statement *body, ast_node_conditional_else_if *else_if, ast_node_compound_statement *else_node);
ast_node_conditional_else_if *create_else_if_node();
ast_node_conditional_else_if *add_else_if_node(ast_node_conditional_else_if *parent, ast_node_expression *condition, ast_node_compound_statement *body);
ast_node_loop_for *create_loop_for_node(ast_node_variable *init, ast_node_expression *start, ast_node_expression *end, ast_node_compound_statement *body);
ast_node_loop_while *create_loop_while_node(ast_node_expression *condition, ast_node_compound_statement *body);
ast_node_loop_control *create_loop_control_node(int node_type);
ast_node_function_def *create_function_def_node(sym_ptr symbol_entry, ast_node_param *params, ast_node_compound_statement *body, ast_node_expression *return_stmt);
ast_node_param *create_parameter_node();
ast_node_param *add_parameter_node(ast_node_param *parent, ast_node_variable *var);
ast_node_function_call *create_function_call_node(sym_ptr symbol, ast_node_arguments *arguments);
ast_node_arguments *create_argument_node();
ast_node_arguments *add_argument_node(ast_node_arguments *parent, ast_node_expression *argument);

void ast_node_dump(ast_node* ast);
void ast_node_type(int node_type);

#endif