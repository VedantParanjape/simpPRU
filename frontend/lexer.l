%{
  #include <iostream>
  #include <vector>

  int linenumber = 0;
  extern int yylex();
%}

%option noyywrap
%x comment

whitespace           (\t|" "|\r)
newline              (\n)
integer              (-?[0-9]+)
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
{integer}               {printf("%s\n", yytext);}
{bool}                  {printf("%s\n", yytext);}
{datatype}              {printf("%s\n", yytext);}
{binary_operators}      {printf("%s\n", yytext);}
{relational_operators}  {printf("%s\n", yytext);}
{assignment_operator}   {printf("%s\n", yytext);}
{identifier}            {printf("%s\n", yytext);}
{terminal}              {printf("%s\n", yytext);}

"/*"            {BEGIN(comment);                              }
<comment>"*/"   {BEGIN(INITIAL);                              }
<comment>\n     {linenumber++;                                }
<comment>.      ;
%%

int main() {
    FILE* fhandle = fopen("test.sim", "r");
    if(fhandle == NULL)
    {
        std::cout << "unable to open file\n";
    }

    yyin = fhandle;
  
    while(yylex());
}

// (^["int"|"bool"]+)[\s+]([a-zA-Z_][\w_]*)[\s*][:][=][\s*]([a-zA-Z_][\w_]*|\d*$)
//https://regex101.com/r/A938rQ/1