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
parenthesis          ("("|")")
integer              ([0-9]+)
bool                 ("true"|"false")
datatype             ("int"|"bool")
arithmetic_operators ("+"|"-"|"*"|"/")
relational_operators (">"|">="|"<"|"<="|"="|"!=")
bitwise_operators    ("~"|"&"|"|")
logical_operators    ("not"|"and"|"or")
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

{parenthesis}           {
  if(!strcmp(yytext, "("))
  {
    return LPAREN;
  }
  else if (!strcmp(yytext, ")"))
  {
    return RPAREN;
  }
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

{bitwise_operators}       {
  if (!strcmp(yytext, "~"))
  {
    return OPR_BW_NOT;
  }
  else if (!strcmp(yytext, "&"))
  {
    return OPR_BW_AND;
  }
  else if (!strcmp(yytext, "|"))
  {
    return OPR_BW_OR;
  }
}

{logical_operators}       {
  if (!strcmp(yytext, "not"))
  {
    return OPR_LGL_NOT;
  }
  else if (!strcmp(yytext, "and"))
  {
    return OPR_LGL_AND;
  }
  else if (!strcmp(yytext, "or"))
  {
    return OPR_LGL_OR;
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