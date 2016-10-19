V                   [A-Za-z][A-Za-z0-9]*
D                   [0-9]
E                   [DEde][-+]?{D}+
TAB                 [ ][ ][ ][ ]              
%option c++
%option noyywrap

%{

    #include "param.tab.hpp"
    #include <string.h>
    static yy::location loc;

    extern yy::parser::semantic_type* yylval;
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}

%%


%{
  // Code run each time yylex is called.
    loc.step ();
%}


\"(\\.|[^\\"])*\"   { 
          yylval->stringVal = new std::string(strdup(yytext));           
                return(yy::parser::token::STRING);}
{D}+    { /*yylval.build(atoi(yytext));*/ 
         long n = strtol(yytext,NULL,10);
         yylval->integerVal = n; return yy::parser::token::INTEGER; }
{D}+"."{D}*({E})?   |
{D}*"."{D}+({E})?   |
{D}+{E} {
        double d = strtod(yytext,NULL);
        yylval->doubleVal = d; return yy::parser::token::DOUBLE; }
{V}     {yylval->var = new std::string(strdup(yytext));  return yy::parser::token::VARIABLE;}
:       {return yy::parser::token::COLON;}
\n      {loc.lines (yyleng); loc.step (); return (yy::parser::token::ENDLINE);}
\t      |
{TAB}   {return yy::parser::token::TAB;}
[ ]     ;
%%

