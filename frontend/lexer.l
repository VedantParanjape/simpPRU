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
braces               ("{"|"}")
parenthesis          ("("|")")
integer              ([0-9]+)
bool                 ("true"|"false")
datatype             ("int"|"bool")
conditional          ("if"|"elif"|"else")
loops                ("for"|"in"|"while")
arithmetic_operators ("+"|"-"|"*"|"/")
relational_operators (">"|">="|"<"|"<="|"="|"!=")
bitwise_operators    ("~"|"&"|"|")
logical_operators    ("not"|"and"|"or")
assignment_operator  ([:][=])
identifier           ([a-zA-Z_][a-zA-Z0-9_]*)
terminal             ([;])
separator            ([:])
%%

{whitespace}            {
  ;
}

{newline}               {
  linenumber++;
}

{braces}                {
  if (!strcmp(yytext, "{"))
  {
    return LBRACE;
  }
  else if (!strcmp(yytext, "}"))
  {
    return RBRACE;
  }
}

{parenthesis}           {
  if (!strcmp(yytext, "("))
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

{conditional}          {
  if (!strcmp(yytext, "if"))
  {
    return KW_IF;
  }
  else if (!strcmp(yytext, "elif"))
  {
    return KW_ELIF;
  }
  else if (!strcmp(yytext, "else"))
  {
    return KW_ELSE; 
  }
}

{loops}            {
  if (!strcmp(yytext, "for"))
  {
    return KW_FOR;
  }
  else if (!strcmp(yytext, "in"))
  {
    return KW_IN;
  }
  else if (!strcmp(yytext, "while"))
  {
    return KW_WHILE;
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

{separator}             {
  return COLON;
}

"/*"            {BEGIN(comment);                              }
<comment>"*/"   {BEGIN(INITIAL);                              }
<comment>\n     {linenumber++;                                }
<comment>.      ;
%%