/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Skeleton interface for Bison GLR parsers in C

   Copyright (C) 2002-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
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
    NEG = 266,
    OPR_GT = 267,
    OPR_LT = 268,
    OPR_EQ = 269,
    OPR_NE = 270,
    OPR_GE = 271,
    OPR_LE = 272,
    OPR_BW_NOT = 273,
    OPR_BW_AND = 274,
    OPR_BW_OR = 275,
    OPR_LGL_NOT = 276,
    OPR_LGL_AND = 277,
    OPR_LGL_OR = 278,
    OPR_ASSIGNMENT = 279,
    SEMICOLON = 280,
    COLON = 281,
    DT_INT = 282,
    DT_BOOL = 283,
    KW_IF = 284,
    KW_ELIF = 285,
    KW_ELSE = 286,
    CONST_INT = 287,
    CONST_BOOL = 288,
    IDENTIFIER = 289
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 16 "parser.y" /* glr.c:197  */

    int integer;
    int boolean;
    char* identifier;

#line 95 "parser.tab.h" /* glr.c:197  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
