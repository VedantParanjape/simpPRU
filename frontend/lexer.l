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
arithmetic_operators ("+"|"-"|"*"|"/")
relational_operators (">"|">="|"<"|"<="|"="|"!=")
assignment_operator  ([:][=])
identifier           ([a-zA-Z_][a-zA-Z0-9_]*)
terminal             ([;])
%%

{whitespace}            {
  ;
}

{newline}               {
  linenumber++;
}

{integer}               {
  yylval.integer = atoi(yytext);
  return CONST_INT;
}

{bool}                  {
  if (!strcmp(yytext, "true"))
  {
    yylval.boolean = 1;
  }
  else if (!strcmp(yytext, "false"))
  {
    yylval.boolean = 0;
  }

  return CONST_BOOL;
}

{datatype}              {
  if (!strcmp(yytext, "int"))
  {
    return DT_INT;
  }
  else if (!strcmp(yytext, "bool"))
  {
    return DT_BOOL;
  }
}

{arithmetic_operators}  {
  if (!strcmp(yytext,"+"))
  {
    return OPR_ADD;
  }
  else if (!strcmp(yytext,"-"))
  {
    return OPR_SUB;
  }
  else if (!strcmp(yytext,"*"))
  {
    return OPR_MUL;
  }
  else if (!strcmp(yytext,"/"))
  {
    return OPR_DIV;
  }
}

{relational_operators}  {
  if (!strcmp(yytext,">"))
  {
    return OPR_GT;
  } 
  else if (!strcmp(yytext, ">="))
  {
    return OPR_GE;
  }
  else if (!strcmp(yytext,"<"))
  {
    return OPR_LT;
  }
  else if (!strcmp(yytext, "<="))
  {
    return OPR_LE;
  }
  else if (!strcmp(yytext,"="))
  {
    return OPR_EQ;
  }
  else if (!strcmp(yytext, "!="))
  {
    return OPR_NE;
  }
}

{assignment_operator}   {
  return OPR_ASSIGNMENT;  
}

{identifier}            {
  yylval.identifier = strdup(yytext);
  return IDENTIFIER;
}

{terminal}              {
  return SEMICOLON;
}

"/*"            {BEGIN(comment);                              }
<comment>"*/"   {BEGIN(INITIAL);                              }
<comment>\n     {linenumber++;                                }
<comment>.      ;
%%