%{
  #include <stdio.h>
  #include "parser.tab.h"

  int linenumber = 0;
  
  void yyerror(char* s);
  int yylex();
%}

%option noyywrap
%x comment

whitespace           (\t|" "|\r)
newline              (\n)
integer              ([-]?[0-9]+)
bool                 ("true"|"false")
datatype             ("int"|"bool")
binary_operators     ("+"|"-"|"*"|"/")
relational_operators (">"|">="|"<"|"<="|"="|"!=")
assignment_operator  ([:][=])
identifier           ([a-zA-Z_][a-zA-Z0-9_]*)
terminal             (";")
%%

{whitespace}            {;}
{newline}               {linenumber++;}
{integer}               {printf("%s\n", yytext); yylval.integer = atoi(yytext); return _integer;                        }
{bool}                  {printf("%s\n", yytext); yylval.integer = atoi(yytext); return _boolean;                        }
{datatype}              {printf("%s\n", yytext); yylval.character_token = strdup(yytext); return _datatype;             }
{binary_operators}      {printf("%s\n", yytext); yylval.character_token = strdup(yytext); return _binary_operators;     }
{relational_operators}  {printf("%s\n", yytext); yylval.character_token = strdup(yytext); return _relational_operators; }
{assignment_operator}   {printf("%s\n", yytext); yylval.character_token = strdup(yytext); return _relational_operators; }
{identifier}            {printf("%s\n", yytext); yylval.character_token = strdup(yytext); return _identifier;           }
{terminal}              {printf("%s\n", yytext); yylval.character_token = strdup(yytext); return _terminal;             }

"/*"            {BEGIN(comment);                              }
<comment>"*/"   {BEGIN(INITIAL);                              }
<comment>\n     {linenumber++;                                }
<comment>.      ;
%%