%{ 
/* analisador léxico para uma calculadora */
/* com suporte a definição de variáveis */
#include "parser.h"
%}

%option yylineno
%option noyywrap

digito	[0-9]
letra_	[a-zA-Z_]
pontdig {digito}*"."?{digito}+
digpont {digito}+"."
pow     "pow("
sqrt    "sqrt("
numero  {pontdig}|{digpont}
id		{letra_}({letra_}|{digito})*

%%

[\t ]+		{ ; /* ignora espaço em branco */ }
{numero}    { yylval.num = atof(yytext); return NUM; }
{id}		{ strcpy(yylval.id, yytext); return ID; }
.|\n		{ return yytext[0]; }
{pow}       { return POW; }
{sqrt}      { return SQRT; }
"=="     { return EQ; }
"!="     { return NE; }
"<="     { return LE; }
">="     { return GE; }
"if("    {return IF;  }
"print(" {return PRINT; }

%%
