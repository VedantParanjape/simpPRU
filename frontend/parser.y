%{
#include <stdlib.h>
#include <stdio.h>

void yyerror(const char* s);
extern int yylex();
extern int yyparse();
extern FILE* yyin;

#define YYDEBUG 1
%}

%define parse.error verbose
%glr-parser 

%union{
    int integer;
    int boolean;
    char* identifier;
}

%left LBRACE RBRACE

%left LPAREN RPAREN

%left OPR_ADD OPR_SUB
%left OPR_MUL OPR_DIV
%left NEG

%left OPR_GT OPR_LT OPR_EQ OPR_NE OPR_GE OPR_LE

%left OPR_BW_NOT OPR_BW_AND OPR_BW_OR

%left OPR_LGL_NOT OPR_LGL_AND OPR_LGL_OR

%token OPR_ASSIGNMENT

%token SEMICOLON COLON COMMA

%token DT_INT
%token DT_BOOL

%token KW_IF KW_ELIF KW_ELSE

%token KW_FOR KW_IN KW_WHILE

%token <integer> CONST_INT
%token <boolean> CONST_BOOL

%token <identifier> IDENTIFIER

%type <integer> arithmetic_expression
%type <boolean> boolean_expression relational_expression logical_expression bool_expressions
%%

translational_unit: statement_list
                  ;

compound_statement: LBRACE statement_list RBRACE
                  ;

statement_list: statement
              | statement_list statement
              ;

statement: empty_statement
         | declaration
         | declaration_assignment
         | assignment
         | conditional_statement
         | loop_statement_for
         | loop_statement_while
         ;

empty_statement: SEMICOLON {
              printf ("blank statement\n");
            }
            ;

declaration: DT_INT IDENTIFIER SEMICOLON { 
               printf ("int %s ;\n", $2);
               free($2);
           }
           | DT_BOOL IDENTIFIER SEMICOLON {
               printf ("bool %s ;\n", $2);
               free($2);
           }
           ;

declaration_assignment: DT_INT IDENTIFIER OPR_ASSIGNMENT arithmetic_expression SEMICOLON {
               printf ("%s := %d\n", $2, $4);
               free($2);
            }
            | DT_BOOL IDENTIFIER OPR_ASSIGNMENT bool_expressions SEMICOLON {
               printf ("%s := %d\n", $2, $4);
               free($2);
            }
            ;

assignment: IDENTIFIER OPR_ASSIGNMENT arithmetic_expression SEMICOLON {
               printf("%s := %d\n", $1, $3);
               free($1);
            }
            | IDENTIFIER OPR_ASSIGNMENT bool_expressions SEMICOLON {
                printf("%s := %d\n", $1, $3);
                free($1);
            }

bool_expressions: boolean_expression 
                | relational_expression 
                | logical_expression
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
          | LPAREN arithmetic_expression RPAREN {
              $$ = $2;
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
          | LPAREN boolean_expression RPAREN {
              $$ = $2;
          }
          ;

relational_expression: arithmetic_expression OPR_GT arithmetic_expression {
              $$ = $1 > $3;
          }
          | arithmetic_expression OPR_LT arithmetic_expression {
              $$ = $1 < $3;
          }
          | arithmetic_expression OPR_EQ arithmetic_expression {
              $$ = $1 == $3;
          }
          | arithmetic_expression OPR_NE arithmetic_expression {
              $$ = $1 != $3;
          }
          | arithmetic_expression OPR_GE arithmetic_expression {
              $$ = $1 >= $3;
          }
          | arithmetic_expression OPR_LE arithmetic_expression {
              $$ = $1 <= $3;
          }
          | LPAREN relational_expression RPAREN {
              $$ = $2;
          } 
          ;


logical_expression: OPR_LGL_NOT bool_expressions {
              $$ = $2 ? 0 : 1;
          } 
          | bool_expressions OPR_LGL_AND bool_expressions {
              $$ = $1 & $3;
          }
          | bool_expressions OPR_LGL_OR bool_expressions {
              $$ = $1 | $3;
          }
          | LPAREN logical_expression RPAREN {
              $$ = $2;
          }
          ;

conditional_statement: KW_IF COLON bool_expressions compound_statement conditional_statement_else_if conditional_statement_else {
                          printf("inside if\n");
                     }
                     ;  

conditional_statement_else_if: conditional_statement_else_if KW_ELIF COLON bool_expressions compound_statement {
                                 printf("inside else if\n");
                             }
                             | /* empty */    
                             ;

conditional_statement_else: KW_ELSE compound_statement {
                              printf("inside else\n");
                          }
                          | /* empty */
                          ;

loop_statement_for: KW_FOR COLON IDENTIFIER KW_IN CONST_INT COLON CONST_INT compound_statement {
                      printf("inside for => %s => %d : %d\n", $3, $5, $7);
                  }
                  ;

loop_statement_while: KW_WHILE COLON bool_expressions compound_statement {
                      printf("inside while\n");
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
    exit(0);
} 
