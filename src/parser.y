%{
#include <stdlib.h>
#include <stdio.h>
#include "symbol_table.h"
#include "ast.h"
#include "semantic.h"
#include "code_printer.h"

void yyerror(const char* s);
extern int yylex();
extern int yyparse();
extern FILE* yyin;

extern int linenumber;
extern int assignment_flag;
sym_ptr temp = NULL;
ast_node *ast = NULL;

#define YYDEBUG 1
%}

%define parse.error verbose
%glr-parser 

%union{
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
}

%left LBRACE RBRACE

%left LPAREN RPAREN

%left OPR_ADD OPR_SUB
%left OPR_MUL OPR_DIV

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

%token KW_DIGITAL_READ KW_DIGITAL_WRITE KW_DELAY KW_PWM KW_START_COUNTER KW_STOP_COUNTER KW_READ_COUNTER

%token <integer> CONST_INT
%token <boolean> CONST_BOOL

%token <symbol_handle> IDENTIFIER INT_IDENTIFIER BOOL_IDENTIFIER VOID_IDENTIFIER

%type <node> translation_unit program
%type <statements> statement
%type <compound_statement> statement_list compound_statement conditional_statement_else
%type <declaration> declaration declaration_assignment
%type <assignment> assignment
%type <expression> bool_expressions arithmetic_expression boolean_expression relational_expression logical_expression return_statement function_call_datatypes
%type <conditional_if> conditional_statement
%type <conditional_else_if> conditional_statement_else_if
%type <loop_for> loop_statement_for
%type <loop_while> loop_statement_while
%type <function_def> function_definition 
%type <param> parameter_list_def parameters
%type <variable> parameter
%type <function_call> int_function_call bool_function_call void_function_call 
%type <util_function_call> digital_read_call digital_write_call delay_call pwm_call start_counter_call stop_counter_call read_counter_call
%type <arguments> function_call_parameters
%start start
%%

start: translation_unit {
        ast = $1;
     }
     ;

translation_unit: program {
                    $$ = create_translation_unit();
                    $$ = add_program_unit($$, $1);
                }
	            | translation_unit program {
                    $$ = add_program_unit($1, $2);
                }
	            ;

program: statement {
        $$ = (ast_node*)$1;     
       }
       | function_definition {
        $$ = (ast_node*)$1;
       }
       ;

compound_statement: LBRACE statement_list RBRACE {
                    $$ = $2;
                  }
                  ;

statement_list: statement {
                $$ = create_compound_statement_node();
                $$ = add_compound_statement_node($$, $1);
              }
              | statement_list statement {
                  $$  = add_compound_statement_node($1, $2);
              }
              ;

statement: compound_statement {
            $$ = create_statement_node(AST_NODE_COMPOUND_STATEMENT, (void*)$1);
         }
         | empty_statement {
            $$ = create_statement_node(AST_NODE_EMPTY_STATEMENT, NULL); 
         }
         | declaration {
             $$ = create_statement_node(AST_NODE_DECLARATION, (void*)$1);
         }
         | declaration_assignment {
             $$ = create_statement_node(AST_NODE_DECLARATION, (void*)$1);
         }
         | assignment {
             $$ = create_statement_node(AST_NODE_ASSIGNMENT, (void*)$1);
         }
         | conditional_statement {
             $$ = create_statement_node(AST_NODE_CONDITIONAL_IF, (void*)$1);
         }
         | loop_statement_for {
             $$ = create_statement_node(AST_NODE_LOOP_FOR, (void*)$1);
         }
         | loop_statement_while {
             $$ = create_statement_node(AST_NODE_LOOP_WHILE, (void*)$1);
         }
         | return_statement {
             $$ = create_statement_node(AST_NODE_FUNC_RETURN, (void*)$1);
         }
         | int_function_call SEMICOLON {
             $$ = create_statement_node(AST_NODE_FUNC_CALL, (void*)$1);
         }
         | bool_function_call SEMICOLON {
             $$ = create_statement_node(AST_NODE_FUNC_CALL, (void*)$1);
         }
         | void_function_call SEMICOLON {
             $$ = create_statement_node(AST_NODE_FUNC_CALL, (void*)$1);
         }
         | KW_BREAK SEMICOLON {
             $$ = create_statement_node(AST_NODE_LOOP_BREAK, (void*)create_loop_control_node(AST_NODE_LOOP_BREAK));
         }
         | KW_CONTINUE SEMICOLON {
             $$ = create_statement_node(AST_NODE_LOOP_CONTINUE, (void*)create_loop_control_node(AST_NODE_LOOP_BREAK));
         }
         | digital_read_call SEMICOLON {
             $$ = create_statement_node(AST_NODE_DIGITAL_READ_CALL, (void*)$1);
         }
         | digital_write_call SEMICOLON {
             $$ = create_statement_node(AST_NODE_DIGITAL_WRITE_CALL, (void*)$1);
         }
         | delay_call SEMICOLON {
             $$ = create_statement_node(AST_NODE_DELAY_CALL, (void*)$1);
         }
         | pwm_call SEMICOLON {
             $$ = create_statement_node(AST_NODE_PWM_CALL, (void*)$1);
         }
         | start_counter_call SEMICOLON {
             $$ = create_statement_node(AST_NODE_START_COUNTER_CALL, (void*)$1);
         }
         | stop_counter_call SEMICOLON {
             $$ = create_statement_node(AST_NODE_STOP_COUNTER_CALL, (void*)$1);
         }
         | read_counter_call SEMICOLON {
             $$ = create_statement_node(AST_NODE_READ_COUNTER_CALL, (void*)$1);
         }
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
               $$ = create_declaration_node($2, NULL);

               printf ("int %s ;\n", $2->identifier);
           }
           | DT_BOOL IDENTIFIER SEMICOLON {
               if ($2 == NULL)
               {
                   yyerror("variable already defined");
               }

               $2->data_type = DT_BOOLEAN;
               $$ = create_declaration_node($2, NULL);

               printf ("bool %s ;\n", $2->identifier);
           }
           ;
        
declaration_assignment: DT_INT IDENTIFIER OPR_ASSIGNMENT arithmetic_expression SEMICOLON {
               if ($2 == NULL)
               {
                   yyerror("variable already defined");
               }

               $2->data_type = DT_INTEGER;
               $2->value = $4->value;
               $$ = create_declaration_node($2, $4);

               printf ("%s := %d\n", $2->identifier, $2->value);
            }
            | DT_BOOL IDENTIFIER OPR_ASSIGNMENT bool_expressions SEMICOLON {
               if ($2 == NULL)
               {
                   yyerror("variable already defined");
               }

               $2->data_type = DT_BOOLEAN;
               $2->value = $4->value;
               $$ = create_declaration_node($2, $4);

               printf ("%s := %d\n", $2->identifier, $2->value);
            }
            ;

assignment: INT_IDENTIFIER OPR_ASSIGNMENT arithmetic_expression SEMICOLON {
               if ($1 == NULL)
               {
                   yyerror("variable already defined");
               }

               $1->data_type = DT_INTEGER;
               $1->value = $3->value;
               $$ = create_assignment_node($1, $3);

               printf("%s := %d\n", $1->identifier, $1->value);
            }
            | BOOL_IDENTIFIER OPR_ASSIGNMENT bool_expressions SEMICOLON {
                if ($1 == NULL)
                {
                    yyerror("variable already defined");
                }
                
                $1->data_type = DT_BOOLEAN;  
                $1->value = $3->value;
                $$ = create_assignment_node($1, $3);

               printf("%s := %d\n", $1->identifier, $1->value);
            }
            ;
            
bool_expressions: boolean_expression {
                    $$ = $1;
                }
                | relational_expression {
                    $$ = $1;
                }
                | logical_expression {
                    $$ = $1;
                }
                ;

arithmetic_expression: CONST_INT {
              $$ = create_expression_node(AST_NODE_ARITHMETIC_EXP, AST_NODE_CONSTANT, $1, NULL, NULL);
          }
          | INT_IDENTIFIER {
              if ($1 != NULL)
              {
                  if ($1->data_type == DT_INTEGER)
                  {
                      $$ = create_expression_node(AST_NODE_ARITHMETIC_EXP, AST_NODE_VARIABLE, $1->value, NULL, NULL);
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
              $$ = create_expression_node(AST_NODE_ARITHMETIC_EXP, AST_NODE_FUNC_CALL, $1->symbol_entry->value, (ast_node*)$1, NULL);
          }
          | read_counter_call {
              $$ = create_expression_node(AST_NODE_ARITHMETIC_EXP, AST_NODE_READ_COUNTER_CALL, 0, (ast_node*)$1, NULL);
          }
          | arithmetic_expression OPR_ADD arithmetic_expression {
              $$ = create_expression_node(AST_NODE_ARITHMETIC_EXP, AST_OPR_ADD, $1->value + $3->value, (ast_node*)$1, (ast_node*)$3);
          } 
          | arithmetic_expression OPR_SUB arithmetic_expression {
              $$ = create_expression_node(AST_NODE_ARITHMETIC_EXP, AST_OPR_SUB, $1->value - $3->value, (ast_node*)$1, (ast_node*)$3);
          } 
          | arithmetic_expression OPR_MUL arithmetic_expression {
              $$ = create_expression_node(AST_NODE_ARITHMETIC_EXP, AST_OPR_MUL, $1->value * $3->value, (ast_node*)$1, (ast_node*)$3);
          } 
          | arithmetic_expression OPR_DIV arithmetic_expression {
              $$ = create_expression_node(AST_NODE_ARITHMETIC_EXP, AST_OPR_DIV, $1->value / $3->value, (ast_node*)$1, (ast_node*)$3);
          }
          | OPR_SUB arithmetic_expression {
              $$ = create_expression_node(AST_NODE_ARITHMETIC_EXP, AST_OPR_SUB, -1*$2->value, NULL, (ast_node*)$2);
          } 
          | LPAREN arithmetic_expression RPAREN {
              $$ = $2;
          }
          ;

boolean_expression: CONST_BOOL {
              $$ = create_expression_node(AST_NODE_BOOLEAN_EXP, AST_NODE_CONSTANT, $1, NULL, NULL);
          }
          | BOOL_IDENTIFIER {
              if ($1 != NULL)
              {
                  if ($1->data_type == DT_BOOLEAN)
                  {
                      $$ = create_expression_node(AST_NODE_BOOLEAN_EXP, AST_NODE_VARIABLE, $1->value, NULL, NULL);
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
              $$ = create_expression_node(AST_NODE_BOOLEAN_EXP, AST_NODE_FUNC_CALL, $1->symbol_entry->value, (ast_node*)$1, NULL);
          }
          | digital_read_call {
              $$ = create_expression_node(AST_NODE_BOOLEAN_EXP, AST_NODE_DIGITAL_READ_CALL, 0, (ast_node*)$1, NULL);
          }
          | OPR_BW_NOT boolean_expression {
              $$ = create_expression_node(AST_NODE_BOOLEAN_EXP, AST_OPR_BW_NOT, $2->value ? 0 : 1, NULL, (ast_node*)$2);

          }
          | boolean_expression OPR_BW_AND boolean_expression {
              $$ = create_expression_node(AST_NODE_BOOLEAN_EXP, AST_OPR_BW_AND, $1->value & $3->value, (ast_node*)$1, (ast_node*)$3);
          } 
          | boolean_expression OPR_BW_OR boolean_expression {
              $$ = create_expression_node(AST_NODE_BOOLEAN_EXP, AST_OPR_BW_OR, $1->value | $3->value, (ast_node*)$1, (ast_node*)$3);
          }
          | LPAREN boolean_expression RPAREN {
              $$ = $2;
          }
          ;

relational_expression: arithmetic_expression OPR_GT arithmetic_expression {
              $$ = create_expression_node(AST_NODE_RELATIONAL_EXP, AST_OPR_GT, $1->value > $3->value, (ast_node*)$1, (ast_node*)$3);
          }
          | arithmetic_expression OPR_LT arithmetic_expression {
              $$ = create_expression_node(AST_NODE_RELATIONAL_EXP, AST_OPR_LT, $1->value < $3->value, (ast_node*)$1, (ast_node*)$3);
          }
          | arithmetic_expression OPR_EQ arithmetic_expression {
              $$ = create_expression_node(AST_NODE_RELATIONAL_EXP, AST_OPR_EQ, $1->value == $3->value, (ast_node*)$1, (ast_node*)$3);
          }
          | arithmetic_expression OPR_NE arithmetic_expression {
              $$ = create_expression_node(AST_NODE_RELATIONAL_EXP, AST_OPR_NE, $1->value != $3->value, (ast_node*)$1, (ast_node*)$3);
          }
          | arithmetic_expression OPR_GE arithmetic_expression {
              $$ = create_expression_node(AST_NODE_RELATIONAL_EXP, AST_OPR_GE, $1->value >= $3->value, (ast_node*)$1, (ast_node*)$3);
          }
          | arithmetic_expression OPR_LE arithmetic_expression {
              $$ = create_expression_node(AST_NODE_RELATIONAL_EXP, AST_OPR_LE, $1->value <= $3->value, (ast_node*)$1, (ast_node*)$3);
          }
          | LPAREN relational_expression RPAREN {
              $$ = $2;
          } 
          ;


logical_expression: OPR_LGL_NOT bool_expressions {
              $$ = create_expression_node(AST_NODE_LOGICAL_EXP, AST_OPR_LGL_NOT, $2->value ? 0 : 1, NULL, (ast_node*)$2);
          } 
          | bool_expressions OPR_LGL_AND bool_expressions {
              $$ = create_expression_node(AST_NODE_LOGICAL_EXP, AST_OPR_LGL_AND, $1->value & $3->value, (ast_node*)$1, (ast_node*)$3);
          }
          | bool_expressions OPR_LGL_OR bool_expressions {
              $$ = create_expression_node(AST_NODE_LOGICAL_EXP, AST_OPR_LGL_OR, $1->value | $3->value, (ast_node*)$1, (ast_node*)$3);
          }
          | LPAREN logical_expression RPAREN {
              $$ = $2;
          }
          ;

conditional_statement: KW_IF COLON bool_expressions compound_statement conditional_statement_else_if conditional_statement_else {
                          printf("inside if\n");
                          $$ = create_conditional_if_node($3, $4, $5, $6);
                     }
                     ;  

conditional_statement_else_if: conditional_statement_else_if KW_ELIF COLON bool_expressions compound_statement {
                                 printf("inside else if\n");
                                 $$ = add_else_if_node($1, $4, $5);
                             }
                             | /* empty */    {
                                 $$ = create_else_if_node();
                             }
                             ;

conditional_statement_else: KW_ELSE compound_statement {
                              printf("inside else\n");
                              $$ = $2;
                          }
                          | /* empty */ {
                              $$ = NULL;
                          }
                          ;

loop_statement_for: KW_FOR COLON IDENTIFIER KW_IN arithmetic_expression COLON arithmetic_expression compound_statement {
                      $3->data_type = DT_INTEGER;
                      $3->value = $5->value;
                      $$ = create_loop_for_node(create_variable_node(AST_DT_INT, $3), $5, $7, $8);
                      
                      printf("inside for => %s => %d : %d\n", $3->identifier, $5->value, $7->value);
                  }
                  ;

loop_statement_while: KW_WHILE COLON bool_expressions compound_statement {
                      printf("inside while\n");
                      $$ = create_loop_while_node($3, $4);
                    }
                    ;

function_definition: KW_DEF IDENTIFIER COLON DT_INT {
                       if ($2 == NULL){yyerror("function name already defined");}
                       temp = $2; temp->data_type = DT_INTEGER;} 
                   COLON parameters compound_statement {
                       if (vec_last(&$8->child_nodes)->node_type == AST_NODE_FUNC_RETURN)
                       {
                           $$ = create_function_def_node($2, $7, $8, vec_pop(&$8->child_nodes)->child_nodes.return_statement);
                       }
                       else if (vec_last(&$8->child_nodes)->node_type != AST_NODE_FUNC_RETURN && $2->data_type == DT_VOID_)
                       {
                           $$ = create_function_def_node($2, $7, $8, NULL);
                       }
                       else if (vec_last(&$8->child_nodes)->node_type != AST_NODE_FUNC_RETURN && $2->data_type != DT_VOID_)
                       {
                           yyerror("return statement missing in a non void function");
                       }
                       temp = NULL;
                       
                       if (check_function_definition($$) == -1)
                       {
                          yyerror("return statement different from return type");
                       }
                       printf("func\n");
                   }
                   | KW_DEF IDENTIFIER COLON DT_BOOL {
                       if ($2 == NULL){yyerror("function name already defined");}
                       temp = $2; temp->data_type = DT_BOOLEAN;} 
                   COLON parameters compound_statement {
                       if (vec_last(&$8->child_nodes)->node_type == AST_NODE_FUNC_RETURN)
                       {
                           $$ = create_function_def_node($2, $7, $8, vec_pop(&$8->child_nodes)->child_nodes.return_statement);
                       }
                       else if (vec_last(&$8->child_nodes)->node_type != AST_NODE_FUNC_RETURN && $2->data_type == DT_VOID_)
                       {
                           $$ = create_function_def_node($2, $7, $8, NULL);
                       }
                       else if (vec_last(&$8->child_nodes)->node_type != AST_NODE_FUNC_RETURN && $2->data_type != DT_VOID_)
                       {
                           yyerror("return statement missing in a non void function");
                       }
                       temp = NULL;
                       
                       if (check_function_definition($$) == -1)
                       {
                          yyerror("return statement different from return type");
                       }
                       printf("func\n");
                   }
                   | KW_DEF IDENTIFIER COLON DT_VOID {
                       if ($2 == NULL){yyerror("function name already defined");}
                       temp = $2; temp->data_type = DT_VOID_;} 
                   COLON parameters compound_statement {
                       if (vec_last(&$8->child_nodes)->node_type == AST_NODE_FUNC_RETURN)
                       {
                           $$ = create_function_def_node($2, $7, $8, vec_pop(&$8->child_nodes)->child_nodes.return_statement);
                       }
                       else if (vec_last(&$8->child_nodes)->node_type != AST_NODE_FUNC_RETURN && $2->data_type == DT_VOID_)
                       {
                           $$ = create_function_def_node($2, $7, $8, NULL);
                       }
                       else if (vec_last(&$8->child_nodes)->node_type != AST_NODE_FUNC_RETURN && $2->data_type != DT_VOID_)
                       {
                           yyerror("return statement missing in a non void function");
                       }
                       temp = NULL;
                       
                       if (check_function_definition($$) == -1)
                       {
                          yyerror("return statement different from return type");
                       }
                       printf("func\n");
                   }
                   ;

parameters: parameter_list_def {
             $$ = $1; 
         }
         | /* empty */ {
             $$ = NULL;
         }
         ;

parameter_list_def: parameter_list_def COMMA parameter {
                    $$ = add_parameter_node($1, $3);
                  }
                  | parameter {
                      $$ = create_parameter_node();
                      $$ = add_parameter_node($$, $1);
                  }  
                  ;

parameter: DT_INT IDENTIFIER {
            $2->data_type = DT_INTEGER;
            vec_push(&temp->params, $2);

            $$ = create_variable_node(AST_DT_INT, $2);
         }
         | DT_BOOL IDENTIFIER {
            $2->data_type = DT_BOOLEAN;
            vec_push(&temp->params, $2);

            $$ = create_variable_node(AST_DT_BOOL, $2);
         }
         ;

return_statement: KW_RETURN bool_expressions SEMICOLON {
                    $$ = $2;
                }
                | KW_RETURN arithmetic_expression SEMICOLON {
                    $$ = $2;
                }
                | KW_RETURN SEMICOLON {
                    $$ = NULL;
                }
                ;

int_function_call: INT_IDENTIFIER LPAREN function_call_parameters RPAREN {
                if ($1 != NULL)
                {
                    if ($1->is_function != 1)
                    {
                        yyerror("not a function");
                    }

                    $$ = create_function_call_node($1, $3);
                    if(check_function_call($$) == -1)
                    {
                        yyerror("wrong paramters for the function");
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
                    
                    $$ = create_function_call_node($1, $3);
                    if(check_function_call($$) == -1)
                    {
                        yyerror("wrong paramters for the function");
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

                    $$ = create_function_call_node($1, $3);
                    if(check_function_call($$) == -1)
                    {
                        yyerror("wrong paramters for the function");
                    }
                }
                else 
                {
                    yyerror("function not defined");
                }
                printf("function call\n");
             }
             ;

digital_read_call: KW_DIGITAL_READ LPAREN arithmetic_expression RPAREN {
                        $$ = create_digital_read_call_node($3);
                     }
                     ;

digital_write_call: KW_DIGITAL_WRITE LPAREN arithmetic_expression COMMA bool_expressions RPAREN {
                    $$ = create_digital_write_call_node($3, $5);
                  }
                  ;

delay_call: KW_DELAY LPAREN arithmetic_expression RPAREN {
            $$ = create_delay_call_node($3);
          }
          ;

pwm_call: KW_PWM LPAREN arithmetic_expression COMMA arithmetic_expression RPAREN {
            $$ = create_pwm_call_node($3, $5);
        }
        ;

start_counter_call: KW_START_COUNTER LPAREN RPAREN {
                    $$ = create_start_counter_call_node();
                  }
                  ;

stop_counter_call: KW_STOP_COUNTER LPAREN RPAREN {
                    $$ = create_stop_counter_call_node();
                 }
                 ;

read_counter_call: KW_READ_COUNTER LPAREN RPAREN {
                    $$ = create_read_counter_call_node();
                 }

function_call_parameters: function_call_parameters COMMA function_call_datatypes {
                            $$ = add_argument_node($1, $3);
                        }
                        | function_call_datatypes {
                            $$ = create_argument_node();
                            $$ = add_argument_node($$, $1);
                        }
                        | /* empty */ {
                            $$ = NULL;
                        }
                        ;
                        
function_call_datatypes: arithmetic_expression {
                           $$ = $1;
                       }
                       | boolean_expression {
                           $$ = $1;
                       }
                       ;
%%

int main(int argc, char** argv)
{
    FILE* fhandle = fopen(argv[1], "r");
    yyin = fhandle;
    init_symbol_table();

    while(yyparse());

    dump_symbol_table();
    /* printf("%d",ast->node_type); */

    ast_node_dump(ast);
    code_printer(ast);

    system("pru-gcc ../generated_code/temp.c -o out.pru -mmcu=am335x.pru0");
}

void yyerror (const char *s) 
{
    fprintf (stderr, "%d : error: %s\n", linenumber, s);
    exit(0);
} 
