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
%left NEG

%left OPR_GT OPR_LT OPR_EQ OPR_NE OPR_LE OPR_GE

%left OPR_BW_NOT OPR_BW_AND OPR_BW_OR

%token OPR_ASSIGNMENT

%token SEMICOLON

%token DT_INT
%token DT_BOOL

%token <integer> CONST_INT
%token <boolean> CONST_BOOL

%token <identifier> IDENTIFIER

%type <integer> arithmetic_expression
%type <boolean> boolean_expression
%%

declarations: declarations declaration
            | declaration
            ;

declaration: DT_INT IDENTIFIER SEMICOLON { 
               printf ("int %s ;\n", $2);
               free($2);
           }
           | DT_INT IDENTIFIER OPR_ASSIGNMENT arithmetic_expression SEMICOLON {
               printf ("%s := %d\n", $2, $4);
               free($2);
           } 
           | DT_BOOL IDENTIFIER SEMICOLON {
               printf ("bool %s ;\n", $2);
               free($2);
           }
           | DT_BOOL IDENTIFIER OPR_ASSIGNMENT boolean_expression SEMICOLON {
               printf ("%s := %d\n", $2, $4);
               free($2);
           }
           ;

arithmetic_expression: CONST_INT {
              $$ = $1;
          }
          | arithmetic_expression OPR_ADD arithmetic_expression {
              $$ = $1 + $3;
          } 
          | arithmetic_expression OPR_SUB arithmetic_expression {
              $$ = $1 - $3;
          } 
          | arithmetic_expression OPR_MUL arithmetic_expression {
              $$ = $1 * $3;
          } 
          | arithmetic_expression OPR_DIV arithmetic_expression {
              $$ = $1 / $3;
          }
          | OPR_SUB arithmetic_expression %prec NEG {
              $$ = -$2;
          } 
          ;

boolean_expression: CONST_BOOL {
              $$ = $1;  
          }
          | OPR_BW_NOT boolean_expression {
              $$ = $2 ? 0 : 1;
          }
          | boolean_expression OPR_BW_AND boolean_expression {
              $$ = $1 & $3;
          } 
          | boolean_expression OPR_BW_OR boolean_expression {
              $$ = $1 | $3;
          }
          ;
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
