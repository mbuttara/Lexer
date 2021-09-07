%option nounput
%option noyywrap

%{
#include "token.h"

std::string string_content;

%}


LE [<][=]
GE [>][=]
EQ [=][=]
NEQ [!][=]
AND [&][&]
OR [|][|]

DBLLITERAL [0-9]+\.[0-9]*
INTLITERAL [0-9]+
IDENTIFIER [A-Za-z_][A-Za-z0-9_]*


%x STRING
%x COMMENT


%%
void { return make_token(T_VOID, current_line);}
int  { return make_token(T_INT, current_line); }
double { return make_token(T_DOUBLE, current_line); }
bool { return make_token(T_BOOL, current_line);}
string { return make_token(T_STRING, current_line);}
class {return make_token(T_CLASS, current_line);}
interface {return make_token(T_INTERFACE, current_line);}
null {return make_token(T_NULL, current_line);}
this {return make_token(T_THIS, current_line);}
extends {return make_token(T_EXTENDS, current_line);}
implements {return make_token(T_IMPLEMENTS, current_line);}
for {return make_token(T_FOR, current_line);}
while {return make_token(T_WHILE, current_line);}
if { return make_token(T_IF, current_line);}
else {return make_token(T_ELSE, current_line);}
return {return make_token(T_RETURN, current_line);}
break {return make_token(T_BREAK, current_line);}
New {return make_token(T_NEW, current_line);}
NewArray {return make_token(T_NEWARRAY, current_line);}
Print {return make_token(T_PRINT, current_line);}
ReadInteger {return make_token(T_READINTEGER, current_line);}
ReadLine {return make_token(T_READLINE, current_line);}

{IDENTIFIER} { return make_token(T_IDENTIFIER, current_line, yytext); }
{INTLITERAL} { return make_token(T_INTLITERAL, current_line, yytext); }
{DBLLITERAL} { return make_token(T_DBLLITERAL, current_line, yytext); }

{GE} {return make_token(T_GE, current_line);}
{LE} {return make_token(T_LE, current_line);}

[+-<>/%\\*=![]\(\)\{\}] { return make_token(yytext[0], current_line); }


\" { BEGIN(STRING); }
<STRING>\n { lex_error("Unterminated string literal"); }
<STRING>\" { BEGIN(INITIAL);
             return make_token(T_STRINGLITERAL, current_line, string_content); }
<STRING>[^\n"]* { string_content = yytext; }


\/\* { BEGIN(COMMENT); }
<COMMENT>\*\/ { BEGIN(INITIAL); }
<COMMENT>. {}


<INITIAL,COMMENT>\n { current_line++; }
\/\/.* {}


[ \t] {}

%%
