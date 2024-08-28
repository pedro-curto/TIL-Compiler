%option c++ prefix="til_scanner_" outfile="til_scanner.cpp"
%option stack noyywrap yylineno 8bit debug
%{ 
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "til_parser.tab.h"

// don't change this
#define yyerror LexerError
%}

%x X_STRING X_COMMENT X_STRING_TERMINATOR

%%
  yydebug=1; set_debug(1);

";".*$                 ; /* ignore comments */

"/*"                   yy_push_state(X_COMMENT);
<X_COMMENT>"*/"        yy_pop_state();
<X_COMMENT>"/*"        yy_push_state(X_COMMENT); /* nested comment */
<X_COMMENT>.|\n        ;

 /* Arithmetic expressions */

">="                   return tGE; 
"<="                   return tLE; 
"=="                   return tEQ; 
"!="                   return tNE;
"&&"                   return tAND;
"||"                   return tOR;

 /* Types */

"int"                  return tINT_T;
"double"               return tDOUBLE_T;
"string"               return tSTRING_T;
"void"                 return tVOID_T;

 /* Global symbols */

"external"             return tEXTERNAL;
"forward"              return tFORWARD;
"public"               return tPUBLIC;
"private"              return tPRIVATE;
"var"                  return tVAR;

 /* Instructions */

"block"                return tBLOCK;
"if"                   return tIF;
"loop"                 return tLOOP; 
"stop"                 return tSTOP;
"next"                 return tNEXT;
"return"               return tRETURN;
"print"                return tPRINT;
"println"              return tPRINTLN;

"process"              return tPROCESS;
"with"                 return tWITH;
"unless"               return tUNLESS;
"iterate"              return tITERATE;
"for"                  return tFOR;
"using"                return tUSING;


 /* Expressions */

"read"                 return tREAD; 
"null"                 return tNULL;
"set"                  return tSET;
"index"                return tINDEX;
"objects"              return tOBJECTS;
"sizeof"               return tSIZEOF;

"program"              return tPROGRAM;
"function"             return tFUNCTION;

 /* Strings */

[A-Za-z][A-Za-z0-9]*       yylval.s = new std::string(yytext); return tIDENTIFIER;

\"                          yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\"                yy_pop_state(); return tSTRING;
<X_STRING>\\\"              *yylval.s += yytext + 1;
<X_STRING>\\\\              *yylval.s += yytext + 1;
<X_STRING>\\t               *yylval.s += '\t';
<X_STRING>\\n               *yylval.s += '\n';
<X_STRING>\\r               *yylval.s += '\r';
<X_STRING>\\0               yy_push_state(X_STRING_TERMINATOR);
<X_STRING>\n                yyerror("newline inside string");
<X_STRING>\0                yyerror("null character inside string");
<X_STRING>\\[0-7]{1,3} {
  int sz = std::stoi(yytext+1, nullptr, 8);
  if (sz > 255) yyerror("octal sequence out of range"); 
  *yylval.s += (char) sz;
}
<X_STRING>\\.               *yylval.s += yytext + 1;
<X_STRING>.                 *yylval.s += yytext;


  /* Ignores everything after a null character */
<X_STRING_TERMINATOR>\"     yy_pop_state(); yy_pop_state(); return tSTRING; /* leaves both x_string_terminator and x_string states */
<X_STRING_TERMINATOR>\\\"   ;
<X_STRING_TERMINATOR>\\\\   ;
<X_STRING_TERMINATOR>\n     yyerror("newline after null character");
<X_STRING_TERMINATOR>\0     yyerror("null character after string terminator");
<X_STRING_TERMINATOR>.      ;

 /* Inteiros */
"0x"0*
"0x"[0-9a-fA-F]+           try { yylval.i = std::stoi(yytext, nullptr, 16); return tINTEGER; } catch (std::out_of_range&) {yyerror("Hexadecimal integer out of range");}
"0x"[0-9A-za-z]*           yyerror("Invalid hexadecimal integer");

0|[1-9][0-9]*              try {yylval.i = std::stoi(yytext, nullptr, 10); return tINTEGER;} catch (std::out_of_range&) {yyerror("Decimal integer out of range");}

 /* Reais */

([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)([eE][-+]?[0-9]+)?   try { yylval.d = std::stod(yytext); return tDOUBLE; } catch (std::out_of_range&) {yyerror("Double out of range");}
[0-9]+([eE][-+]?[0-9]+)                             try { yylval.d = std::stod(yytext); return tDOUBLE; } catch (std::out_of_range&) {yyerror("Double out of range");}

 /* Symbols */

[-()<>=+*/%!@~?]       return *yytext;
[ \t\n\r]+             ; /* ignore whitespace */

.                      yyerror("Unknown character");

%%
