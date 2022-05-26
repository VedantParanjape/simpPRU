%{
  #include <stdio.h>
  #include "parser.h"
  #include "symbol_table.h"

  int linenumber = 1;
  int assignment_flag = 0;
  int for_loop_flag = 0;
  int function_flag = 0;
  int inside_loop = 0;
  int brackets = 0;

  void yyerror(char* s);
  int yylex();
  #define YY_USER_ACTION update_loc();
  static void update_loc(){
  static int curr_line = 1;
  static int curr_col  = 1;

  yylloc.first_line   = curr_line;
  yylloc.first_column = curr_col;

  {char * s; for(s = yytext; *s != '\0'; s++){
    if(*s == '\n'){
      curr_line++;
      curr_col = 1;
    }else{
      curr_col++;
    }
  }}

  yylloc.last_line   = curr_line;
  yylloc.last_column = curr_col-1;
}
%}

%option noyywrap
%x comment

whitespace           (\t|" "|\r)
newline              (\n)
braces               ("{"|"}")
parenthesis          ("("|")")
squares              ("["|"]")
integer              ([0-9]+|(("0x"|"0X")[a-fA-F0-9]+))
bool                 ("true"|"false")
char                 ("'"([ -~]|\t|\n|\r|\0)"'")
string               (\"(\\.|[^"\\])*\")
datatype             ("int"|"bool"|"char"|"uint8")
conditional          ("if"|"elif"|"else")
loops                ("for"|"in"|"while"|"break"|"continue")
arithmetic_operators ("+"|"-"|"*"|"/"|"%")
relational_operators (">"|">="|"<"|"<="|"=="|"!=")
bitwise_operators    ("~"|"&"|"|"|">>"|"<<")
logical_operators    ("not"|"and"|"or")
function             ("return"|"void"|"def")
assignment_operator  ([:][=])
io                   ("digital_read"|"digital_write"|"delay"|"pwm"|"start_counter"|"stop_counter"|"read_counter"|"init_message_channel"|"receive_message"|"send_message"|"send_int"|"send_char"|"send_bool"|"send_ints"|"send_chars"|"send_bools")
print                ("print"|"println")
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
    if (for_loop_flag == -1)
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

    if (inside_loop)
    {
      brackets += 1;
    }
  
    return LBRACE;
  }
  else if (!strcmp(yytext, "}"))
  {
    decrement_scope();

    if (inside_loop)
    {
      brackets -= 1;
      if (brackets == 0)
      {
        inside_loop = 0;
      }
    }

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

{squares}               {
  if (!strcmp(yytext, "["))
  {
    return LSQUARE;
  }
  else if (!strcmp(yytext, "]"))
  {
    return RSQUARE;
  }
}

{integer}               {
  /*octal if first digit is 0, hexadecimal if first two characters are 0x or 0X, decimal otherwise*/
  yylval.integer = strtoll(yytext, NULL, 0);
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

{char}                  {
  yylval.integer = (int)yytext[1];
  return CONST_CHAR;
}

{string}                {
  yylval.string = strdup(yytext);
  return CONST_STRING;
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
  else if (!strcmp(yytext, "char"))
  {
    return DT_CHAR;
  }
  else if (!strcmp(yytext, "uint8"))
  {
    return DT_CHAR;
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
    inside_loop = 1;
    increment_scope();

    return KW_FOR;
  }
  else if (!strcmp(yytext, "in"))
  {
    return KW_IN;
  }
  else if (!strcmp(yytext, "while"))
  {
    inside_loop = 1;
    return KW_WHILE;
  }
  else if (!strcmp(yytext, "break"))
  {
    if (!inside_loop)
    {
      yyerror("cannot call break outside loops");
    }
    return KW_BREAK;
  }
  else if (!strcmp(yytext, "continue"))
  {
    if (!inside_loop)
    {
      yyerror("cannot call break outside loops");
    }
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
  else if (!strcmp(yytext, "%"))
  {
    return OPR_MOD;
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
  else if (!strcmp(yytext,"=="))
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
  else if (!strcmp(yytext, "<<"))
  {
    return OPR_BW_LFT;
  }
  else if (!strcmp(yytext, ">>"))
  {
    return OPR_BW_RGT;
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

{io}                    {
  if (!strcmp(yytext, "digital_write"))
  {
    return KW_DIGITAL_WRITE;
  }
  else if (!strcmp(yytext, "digital_read"))
  {
    return KW_DIGITAL_READ;
  }
  else if (!strcmp(yytext, "delay"))
  {
    return KW_DELAY;
  }
  else if (!strcmp(yytext, "pwm"))
  {
    return KW_PWM;
  }
  else if (!strcmp(yytext, "start_counter"))
  {
    return KW_START_COUNTER;
  }
  else if (!strcmp(yytext, "stop_counter"))
  {
    return KW_STOP_COUNTER;
  }
  else if (!strcmp(yytext, "read_counter"))
  {
    return KW_READ_COUNTER;
  }
  else if (!strcmp(yytext, "init_message_channel"))
  {
    return KW_INIT_RPMSG;
  }
  else if (!strcmp(yytext, "receive_message"))
  {
    return KW_RECV_RPMSG;
  }
  else if (!strcmp(yytext, "send_message"))
  {
    return KW_SEND_RPMSG_INT;
  }
  else if (!strcmp(yytext, "send_int"))
  {
    return KW_SEND_RPMSG_INT;
  }
  else if (!strcmp(yytext, "send_char"))
  {
    return KW_SEND_RPMSG_CHAR;
  }
  else if (!strcmp(yytext, "send_bool"))
  {
    return KW_SEND_RPMSG_BOOL;
  }
  else if (!strcmp(yytext, "send_ints"))
  {
    return KW_SEND_RPMSG_INTS;
  }
  else if (!strcmp(yytext, "send_chars"))
  {
    return KW_SEND_RPMSG_CHARS;
  }
  else if (!strcmp(yytext, "send_bools"))
  {
    return KW_SEND_RPMSG_BOOLS;
  }
}

{print}                 {
  if (!strcmp(yytext, "print"))
  {
    return KW_PRINT;
  }
  else if (!strcmp(yytext, "println"))
  {
    return KW_PRINTLN;
  }
}

{identifier}            {
  yylval.symbol_handle = NULL;

  if (function_flag == 1)
  {
    if (insert_symbol_table(yytext, DT_UNDEF, 1, 1) == 1)
    {
      yylval.symbol_handle = lookup_symbol_table(yytext, get_scope());
      vec_init(&yylval.symbol_handle->params);
    }
    function_flag = -1;
    increment_scope();
    
    return IDENTIFIER;
  }

  if (assignment_flag == 1 || for_loop_flag == 1)
  {
    if (insert_symbol_table(yytext, DT_UNDEF, 1, 0) == 1)
    {
      yylval.symbol_handle = lookup_symbol_table(yytext, get_scope());
      assignment_flag = 0;
  
      if (for_loop_flag == 1)
      {
        for_loop_flag = -1;
      }
    }
  }
  else if (assignment_flag == 0)
  {
    int temp = for_loop_flag == -1 ? -1 : 0;
    yylval.symbol_handle = lookup_symbol_table(yytext, get_scope() + temp);
    
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
    else if (yylval.symbol_handle != NULL && yylval.symbol_handle->data_type == DT_CHAR_)
    {
      return CHAR_IDENTIFIER;
    }
    else if (yylval.symbol_handle != NULL && yylval.symbol_handle->data_type == DT_INT_ARR)
    {
      return INT_ARR_IDENTIFIER;
    }
    else if (yylval.symbol_handle != NULL && yylval.symbol_handle->data_type == DT_CHAR_ARR)
    {
      return CHAR_ARR_IDENTIFIER;
    }
    else if (yylval.symbol_handle != NULL && yylval.symbol_handle->data_type == DT_BOOL_ARR)
    {
      return BOOL_ARR_IDENTIFIER;
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
