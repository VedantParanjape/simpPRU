%{
#include <stdlib.h>
#include <stdio.h>

void yyerror(const char* s);
extern int yylex();
extern int yyparse();
extern FILE* yyin;
%}

%define parse.error verbose
%glr-parser 

%union{
    int integer;
    int boolean;
    char* identifier;
}

%left OPR_ADD OPR_SUB
%left OPR_MUL OPR_DIV

%left OPR_GT OPR_LT OPR_EQ OPR_NE OPR_LE OPR_GE

%token OPR_ASSIGNMENT

%token SEMICOLON

%token DT_INT
%token DT_BOOL

%token <integer> CONST_INT
%token <boolean> CONST_BOOL

%token <identifier> IDENTIFIER

%type <integer> arithmetic_expression
/* %type <boolean> boolean_expression */
%%

declarations: declarations declaration
            | declaration
            ;

declaration: DT_INT IDENTIFIER SEMICOLON { 
               printf ("int %s ;\n", $2);
               free($2);
           }
           | DT_INT IDENTIFIER OPR_ASSIGNMENT arithmetic_expressions SEMICOLON {
               printf ("%s := ", $2);
               free($2);
           } 
           | DT_BOOL IDENTIFIER SEMICOLON {
               printf ("bool %s ;\n", $2);
               free($2);
           }
           | DT_BOOL IDENTIFIER OPR_ASSIGNMENT arithmetic_expressions SEMICOLON {
               printf ("%s := ", $2);
               free($2);
           }
           ;

arithmetic_expressions: arithmetic_expressions arithmetic_expression
                      | arithmetic_expression
                      ;

arithmetic_expression: CONST_INT {
              printf ("%d\n", $1);
          }
          | arithmetic_expression OPR_ADD CONST_INT {
              printf ("%d\n", $1 + $3);
          } 
          | arithmetic_expression OPR_SUB CONST_INT {
              printf ("%d\n", $1 - $3);
          } 
          | arithmetic_expression OPR_MUL CONST_INT {
              printf ("%d\n", $1 * $3);
          } 
          | arithmetic_expression OPR_DIV CONST_INT {
              printf ("%d\n", $1 / $3);
          } 
          ;
/* 
boolean_expression: CONST_BOOL {
              printf ("%d\n", $1);  
          } */
%%

int main()
{
    FILE* fhandle = fopen("test.sim", "r");
    yyin = fhandle;

    while(yyparse());
}

void yyerror (const char *s) 
{
    fprintf (stderr, "error: %s\n", s);
} 
