#ifndef AST_H
#define AST_H

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


#endif