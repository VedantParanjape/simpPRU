/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Skeleton interface for Bison GLR parsers in C

   Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_INCLUDE_PARSER_TAB_H_INCLUDED
# define YY_YY_INCLUDE_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    LBRACE = 258,
    RBRACE = 259,
    LPAREN = 260,
    RPAREN = 261,
    OPR_ADD = 262,
    OPR_SUB = 263,
    OPR_MUL = 264,
    OPR_DIV = 265,
    OPR_GT = 266,
    OPR_LT = 267,
    OPR_EQ = 268,
    OPR_NE = 269,
    OPR_GE = 270,
    OPR_LE = 271,
    OPR_BW_NOT = 272,
    OPR_BW_AND = 273,
    OPR_BW_OR = 274,
    OPR_LGL_NOT = 275,
    OPR_LGL_AND = 276,
    OPR_LGL_OR = 277,
    OPR_ASSIGNMENT = 278,
    SEMICOLON = 279,
    COLON = 280,
    COMMA = 281,
    DT_INT = 282,
    DT_BOOL = 283,
    DT_VOID = 284,
    KW_IF = 285,
    KW_ELIF = 286,
    KW_ELSE = 287,
    KW_FOR = 288,
    KW_IN = 289,
    KW_WHILE = 290,
    KW_BREAK = 291,
    KW_CONTINUE = 292,
    KW_RETURN = 293,
    KW_DEF = 294,
    KW_DIGITAL_READ = 295,
    KW_DIGITAL_WRITE = 296,
    KW_DELAY = 297,
    KW_PWM = 298,
    KW_START_COUNTER = 299,
    KW_STOP_COUNTER = 300,
    KW_READ_COUNTER = 301,
    KW_INIT_RPMSG = 302,
    KW_RECV_RPMSG = 303,
    KW_SEND_RPMSG = 304,
    CONST_INT = 305,
    CONST_BOOL = 306,
    IDENTIFIER = 307,
    INT_IDENTIFIER = 308,
    BOOL_IDENTIFIER = 309,
    VOID_IDENTIFIER = 310
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 25 "parser.y"

    int integer;
    int boolean;
    struct symbol* symbol_handle;
    struct ast_node *node;
    struct ast_node_statements *statements;
    struct ast_node_compound_statement *compound_statement;
    struct ast_node_declaration *declaration;
    struct ast_node_assignment *assignment;
    struct ast_node_expression *expression;
    struct ast_node_constant *constant;
    struct ast_node_variable *variable;
    struct ast_node_conditional_if *conditional_if;
    struct ast_node_conditional_else_if *conditional_else_if;
    struct ast_node_loop_for *loop_for;
    struct ast_node_loop_while *loop_while;
    struct ast_node_loop_control *loop_control;
    struct ast_node_function_def *function_def;
    struct ast_node_param *param;
    struct ast_node_function_call *function_call;
    struct ast_node_arguments *arguments;
    struct ast_node_utility_function_call *util_function_call;

#line 133 "./../include/parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INCLUDE_PARSER_TAB_H_INCLUDED  */
