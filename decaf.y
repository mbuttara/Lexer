/* HI! These are the tokens.
   By convention (in CS 310) they have numbers starting with 260. Take care that
   this list exactly matches the array of strings declared in token.cc. These tokens are only used for multi-character tokens. Single-character tokens
   map to their characters directly.
*/

%token T_VOID 260 "void"
%token T_INT 261 "int"
%token T_DOUBLE 262 "double"
%token T_BOOL 263 "bool"
%token T_STRING 264 "string"
%token T_CLASS 265 "class"
%token T_INTERFACE 266 "interface"
%token T_NULL 267 "null"
%token T_THIS 268 "this"
%token T_EXTENDS 269 "extends"
%token T_IMPLEMENTS 270 "implements"
%token T_FOR 271 "for"
%token T_WHILE 272 "while"
%token T_IF 273 "if"
%token T_ELSE 274 "else"
%token T_RETURN 275 "return"
%token T_BREAK 276 "break"
%token T_NEW 277 "New"
%token T_NEWARRAY 278 "NewArray"
%token T_PRINT 279 "Print"
%token T_READINTEGER 280 "ReadInteger"
%token T_READLINE 281 "ReadLine"


%token T_LE 282
%token T_GE 283
%token T_EQ 284
%token T_NEQ 285
%token T_AND 286
%token T_OR 287
%token T_ARRAY 288

%token T_BOOLLITERAL 289
%token T_INTLITERAL 290
%token T_DBLLITERAL 291
%token T_STRINGLITERAL 292

%token T_TYPEIDENTIFIER 293
%token T_IDENTIFIER 294




%{
#include "parsetree.h"
#define YYSTYPE parse_tree *
int yylex();
extern bool semantic_checks; // defined in the compiler main file.

/* We need this to see syntax errors. */
int yyerror(char const *s)
{
   std::cout << "line " << current_line << ": ";
   std::cout << s << std::endl;
   // exit at the first error.
   exit(1);
}

%}

%define parse.error verbose

%%

/* Debugging hint: if you want to test part of the grammar in isolation,
* change this line rather than using the %start directive from yacc/bison.
* (Crucially, this line sets the "top" variable.)
*/
pgm: program { top = $$ = $1; }

/* Language grammar follows:
*/

/* This is a stub. We are not discussing parsing yet. */
program: T_INT { $$ = new parse_tree(mytok); }


%%
