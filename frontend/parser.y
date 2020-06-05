%{
#include <stdlib.h>
#include <stdio.h>

void yyerror(char* s);
extern int yylex();
extern int yyparse();
extern FILE* yyin;
%}

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

%type <integer> declaration
%%

declaration: DT_INT IDENTIFIER SEMICOLON { 
               printf ("int %s ;\n", $2);
               free($2);
           }
           /* | DT_INT IDENTIFIER OPR_ASSIGNMENT expression SEMICOLON {
               printf ("int %s := exp ;\n", $2);
               free($2);
           }  */
           | DT_BOOL IDENTIFIER SEMICOLON {
               printf ("bool %s ;\n", $2);
               free($2);
           }
           /* | DT_BOOL IDENTIFIER OPR_ASSIGNMENT expression SEMICOLON {
               printf ("bool %s := exp ;\n", $2);
               free($2);
           } */
           ;
/* 
expression: CONST_INT {
              printf ("%d\n", $1);
          }
          | expression OPR_ADD CONST_INT {
              printf ("%d\n", $1 + $3);
          } 
          | expression OPR_SUB CONST_INT {
              printf ("%d\n", $1 - $3);
          } 
          | expression OPR_MUL CONST_INT {
              printf ("%d\n", $1 * $3);
          } 
          | expression OPR_DIV CONST_INT {
              printf ("%d\n", $1 / $3);
          } 
          ; */
%%

int main()
{
    FILE* fhandle = fopen("test.sim", "r");
    yyin = fhandle;

    while(yyparse());
}

void yyerror (char *s) 
{
    fprintf (stderr, "error: %s\n", s);
} 