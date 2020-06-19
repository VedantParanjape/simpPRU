%{
  #include <stdio.h>
  #include "parser.tab.h"
  #include "symbol_table.h"

  int linenumber = 0;
  int assignment_flag = 0;
  int for_loop_flag = 0;
  int function_flag = 0;

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
loops                ("for"|"in"|"while"|"break"|"continue")
arithmetic_operators ("+"|"-"|"*"|"/")
relational_operators (">"|">="|"<"|"<="|"="|"!=")
bitwise_operators    ("~"|"&"|"|")
logical_operators    ("not"|"and"|"or")
function             ("return"|"void"|"def")
assignment_operator  ([:][=])
identifier           ([a-zA-Z_][a-zA-Z0-9_]*)
terminal             ([;])
separator            ([:])
comma                ([,])
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
    if (for_loop_flag == 1)
    {
      for_loop_flag = 0;
    }
    else if (function_flag == -1)
    {
      function_flag = 0;
    }
    else if (for_loop_flag == 0)
    {
      increment_scope();
    }
  
    return LBRACE;
  }
  else if (!strcmp(yytext, "}"))
  {
    decrement_scope();

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
  assignment_flag = 1;

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
    for_loop_flag = 1;
    increment_scope();

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
  else if (!strcmp(yytext, "break"))
  {
    return KW_BREAK;
  }
  else if (!strcmp(yytext, "continue"))
  {
    return KW_CONTINUE;
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

{function}              {
  if (!strcmp(yytext, "return"))
  {
    return KW_RETURN;
  }
  else if (!strcmp(yytext, "void"))
  {
    return DT_VOID;
  }
  else if (!strcmp(yytext, "def"))
  {
    function_flag = 1;

    return KW_DEF;
  }
}

{assignment_operator}   {
  return OPR_ASSIGNMENT;  
}

{identifier}            {
  yylval.symbol_handle = NULL;

  if (function_flag == 1)
  {
    if (insert_symbol_table(yytext, DT_UNDEF, 0, 1) == 1)
    {
      yylval.symbol_handle = lookup_symbol_table(yytext, get_scope());
      vec_init(&yylval.symbol_handle->params);
    }
    function_flag = -1;
    increment_scope();
    
    return IDENTIFIER;
  }

  if (insert_symbol_table(yytext, DT_UNDEF, 0, 0) == 1 && (assignment_flag == 1 || for_loop_flag == 1))
  {
    yylval.symbol_handle = lookup_symbol_table(yytext, get_scope());
    assignment_flag = 0;
  }
  else if (assignment_flag == 0)
  {
    yylval.symbol_handle = lookup_symbol_table(yytext, get_scope());

    if (yylval.symbol_handle != NULL && yylval.symbol_handle->data_type == DT_INTEGER)
    {
      return INT_IDENTIFIER;
    }
    else if (yylval.symbol_handle != NULL && yylval.symbol_handle->data_type == DT_BOOLEAN)
    {
      return BOOL_IDENTIFIER;
    }
    else if (yylval.symbol_handle != NULL && yylval.symbol_handle->data_type == DT_VOID_)
    {
      return VOID_IDENTIFIER;
    }
  }

  return IDENTIFIER;
}

{terminal}              {
  return SEMICOLON;
}

{separator}             {
  return COLON;
}

{comma}                 {
  return COMMA;
}

"/*"            {BEGIN(comment);                              }
<comment>"*/"   {BEGIN(INITIAL);                              }
<comment>\n     {linenumber++;                                }
<comment>.      ;
%%