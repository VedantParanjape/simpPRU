%{
#include <stdlib.h>
#include <stdio.h>

void yyerror(char* s);
extern int yylex();
extern int yyparse();
%}

%union{
    int integer;
    int boolean;
    const char* character_token
}
// %start line

%token <integer> _integer
%token <boolean> _boolean
%token <character_token> _datatype
%token <character_token> _binary_operators
%token <character_token> _relational_operators
%token <character_token> _assignment_operator
%token <character_token> _identifier
%token <character_token> _terminal

%type <integer> expression term
%type <character_token> assignment
%%

// line: 
//     assignment ';' {
//         ;
//     }
//     ;

assignment:
    _identifier ':''=' expression {
        printf("expression: ");
    }
    ;

expression
    : term {
        printf("term: ");
    }
    | expression '+' term {
        printf("expression + term: ");
    }
    | expression '-' term {
        printf("expression - term: ");
    }
    | expression '*' term {
        printf("expression * term: ");
    }
    | expression '/' term {
        printf("expression / term: ");
    }
    | expression '>' term {
        printf("expression > term: ");
    }
    | expression '>''=' term {
        printf("expression >= term: ");
    }
    | expression '<' term {
        printf("expression < term: ");
    }
    | expression '<''=' term {
        printf("expression <= term: ");
    }
    | expression '=' term {
        printf("expression = term: ");
    }
    | expression '!''=' term {
        printf("expression != term: ");
    }
    ;

term
    : _integer {
        printf("number: %d\n", $1);
    }
    | _identifier {
        printf("identifier: %s\n", $1);
    }
    ;
%%

int main()
{
    while(1)
    {
        yyparse();
    }
}

void yyerror (char *s) 
{
    fprintf (stderr, "error: %s\n", s);
} 