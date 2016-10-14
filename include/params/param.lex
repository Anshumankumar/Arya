
V                   [A-Za-z][A-Za-z0-9]*
D                   [0-9]
E                   [DEde][-+]?{D}+
TAB                 [ ][ ][ ][ ]              
%%

%{
#include <stdio.h>
#include "token.h"

%}

\"(\\.|[^\\"])*\"   return(STRING);
{D}+    {ECHO; return (INTEGER);}
{D}+"."{D}*({E})?   |
{D}*"."{D}+({E})?   |
{D}+{E} return (DOUBLE);
{V}     return (VARIABLE);
:       return (COLON);
\n      return (NEWLINE);
\t      |
{TAB}   return (TAB);
[ ]     {}
