%{
#include <stdlib.h>
#include <stdio.h>
#include "symbol_table.h"

void yyerror(const char* s);
extern int yylex();
extern int yyparse();
extern FILE* yyin;

extern int linenumber;
extern int assignment_flag;
sym_ptr temp = NULL;

#define YYDEBUG 1
%}

%define parse.error verbose
%glr-parser 

%union{
    int integer;
    int boolean;
    struct symbol* symbol_handle;
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
%token DT_VOID

%token KW_IF KW_ELIF KW_ELSE

%token KW_FOR KW_IN KW_WHILE KW_BREAK KW_CONTINUE

%token KW_RETURN KW_DEF

%token <integer> CONST_INT
%token <boolean> CONST_BOOL

%token <symbol_handle> IDENTIFIER INT_IDENTIFIER BOOL_IDENTIFIER VOID_IDENTIFIER

%type <integer> arithmetic_expression 
%type <boolean> boolean_expression relational_expression logical_expression bool_expressions 
%type <symbol_handle> int_function_call bool_function_call
%%

translation_unit: program
	            | translation_unit program
	            ;

program: statement
       | function_definition
       ;

compound_statement: LBRACE statement_list RBRACE
                  ;

statement_list: statement
              | statement_list statement
              ;

statement: compound_statement
         | empty_statement
         | declaration
         | declaration_assignment
         | assignment
         | conditional_statement
         | loop_statement_for
         | loop_statement_while
         | return_statement
         | int_function_call SEMICOLON
         | bool_function_call SEMICOLON
         | void_function_call SEMICOLON
         | KW_BREAK SEMICOLON
         | KW_CONTINUE SEMICOLON
         ;

empty_statement: SEMICOLON {
              printf ("blank statement\n");
            }
            ;

declaration: DT_INT IDENTIFIER SEMICOLON { 
               if ($2 == NULL)
               {
                   yyerror("variable already defined");
               }

               $2->data_type = DT_INTEGER;

               printf ("int %s ;\n", $2->identifier);
           }
           | DT_BOOL IDENTIFIER SEMICOLON {
               if ($2 == NULL)
               {
                   yyerror("variable already defined");
               }

               $2->data_type = DT_BOOLEAN;
               
               printf ("bool %s ;\n", $2->identifier);
           }
           ;

declaration_assignment: DT_INT IDENTIFIER OPR_ASSIGNMENT arithmetic_expression SEMICOLON {
               if ($2 == NULL)
               {
                   yyerror("variable already defined");
               }

               $2->data_type = DT_INTEGER;
               $2->value = $4;

               printf ("%s := %d\n", $2->identifier, $2->value);
            }
            | DT_BOOL IDENTIFIER OPR_ASSIGNMENT bool_expressions SEMICOLON {
               if ($2 == NULL)
               {
                   yyerror("variable already defined");
               }

               $2->data_type = DT_BOOLEAN;
               $2->value = $4;

               printf ("%s := %d\n", $2->identifier, $2->value);
            }
            ;

assignment: INT_IDENTIFIER OPR_ASSIGNMENT arithmetic_expression SEMICOLON {
               if ($1 == NULL)
               {
                   yyerror("variable already defined");
               }

               $1->data_type = DT_INTEGER;
               $1->value = $3;
            
               printf("%s := %d\n", $1->identifier, $1->value);
            }
            | BOOL_IDENTIFIER OPR_ASSIGNMENT bool_expressions SEMICOLON {
                if ($1 == NULL)
                {
                    yyerror("variable already defined");
                }
                
                $1->data_type = DT_BOOLEAN;  
                $1->value = $3;
            
               printf("%s := %d\n", $1->identifier, $1->value);
            }
            ;
            
bool_expressions: boolean_expression 
                | relational_expression 
                | logical_expression
                ;

arithmetic_expression: CONST_INT {
              $$ = $1;
          }
          | INT_IDENTIFIER {
              if ($1 != NULL)
              {
                  if ($1->data_type == DT_INTEGER)
                  {
                      $$ = $1->value;
                  }
                  else if ($1->data_type == DT_UNDEF)
                  {
                      yyerror("variable undefined");
                  }
                  else
                  {
                      yyerror("bool variable not allowed with int");
                  }
              }
          } 
          | int_function_call {
              $$ = $1->value;
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
          | BOOL_IDENTIFIER {
              if ($1 != NULL)
              {
                  if ($1->data_type == DT_BOOLEAN)
                  {
                      $$ = $1->value;
                  }
                  else if ($1->data_type == DT_UNDEF)
                  {
                      yyerror("variable undefined");
                  }
                  else
                  {
                      yyerror("int variable not allowed with bool");
                  }
              }
          }
          | bool_function_call {
              $$ = $1->value;
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
                      $3->data_type = DT_INTEGER;
                      $3->value = $5;
                    
                      printf("inside for => %s => %d : %d\n", $3->identifier, $5, $7);
                  }
                  ;

loop_statement_while: KW_WHILE COLON bool_expressions compound_statement {
                      printf("inside while\n");
                    }
                    ;

function_definition: KW_DEF IDENTIFIER COLON DT_INT {
                       if ($2 == NULL){yyerror("function name already defined");}
                       temp = $2; temp->data_type = DT_INTEGER;} 
                   COLON parameters compound_statement {
                       temp = NULL;
                       printf("func\n");
                   }
                   | KW_DEF IDENTIFIER COLON DT_BOOL {
                       if ($2 == NULL){yyerror("function name already defined");}
                       temp = $2; temp->data_type = DT_BOOLEAN;} 
                   COLON parameters compound_statement {
                       temp = NULL;
                       printf("func\n");
                   }
                   | KW_DEF IDENTIFIER COLON DT_VOID {
                       if ($2 == NULL){yyerror("function name already defined");}
                       temp = $2; temp->data_type = DT_VOID_;} 
                   COLON parameters compound_statement {
                       temp = NULL;
                       printf("func\n");
                   }
                   ;

parameters: parameter_list_def
         | /* empty */
         ;

parameter_list_def: parameter_list_def COMMA parameter
                  | parameter     
                  ;

parameter: DT_INT IDENTIFIER {
            $2->data_type = DT_INT;
            vec_push(&temp->params, $2);
         }
         | DT_BOOL IDENTIFIER {
            $2->data_type = DT_BOOL;
            vec_push(&temp->params, $2);
         }
         ;

return_statement: KW_RETURN bool_expressions SEMICOLON
                | KW_RETURN arithmetic_expression SEMICOLON
                | KW_RETURN SEMICOLON
                ;

int_function_call: INT_IDENTIFIER LPAREN function_call_parameters RPAREN {
                if ($1 != NULL)
                {
                    if ($1->is_function != 1)
                    {
                        yyerror("not a function");
                    }
                }
                else 
                {
                    yyerror("function not defined");
                }
                printf("function call\n");
             }
             ;

bool_function_call: BOOL_IDENTIFIER LPAREN function_call_parameters RPAREN {
                if ($1 != NULL)
                {
                    if ($1->is_function != 1)
                    {
                        yyerror("not a function");
                    }
                }
                else 
                {
                    yyerror("function not defined");
                }
                printf("function call\n");
             }
             ;

void_function_call: VOID_IDENTIFIER LPAREN function_call_parameters RPAREN {
                if ($1 != NULL)
                {
                    if ($1->is_function != 1)
                    {
                        yyerror("not a function");
                    }
                }
                else 
                {
                    yyerror("function not defined");
                }
                printf("function call\n");
             }
             ;

function_call_parameters: function_call_parameters COMMA function_call_datatypes
                        | function_call_datatypes
                        | /* empty */
                        ;
                        
function_call_datatypes: arithmetic_expression
                       | boolean_expression
                       ;
%%

int main()
{
    FILE* fhandle = fopen("test.sim", "r");
    yyin = fhandle;
    init_symbol_table();

    while(yyparse());

    dump_symbol_table();
}

void yyerror (const char *s) 
{
    fprintf (stderr, "%d : error: %s\n", linenumber, s);
    exit(0);
} 
