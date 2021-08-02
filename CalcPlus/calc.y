%{
/* analisador sintático para uma calculadora */
/* com suporte a definição de variáveis */
#include <iostream>
#include <string>
#include <unordered_map>
#include <math.h>

using std::string;
using std::unordered_map;
using std::cout;

/* protótipos das funções especiais */
int yylex(void);
int yyparse(void);
void yyerror(const char *);

/* tabela de símbolos */
unordered_map<string,double> variables;
%}

%union {
	double num;
	char id[16];
}

%token <id> ID
%token <num> NUM
%token POW
%token SQRT
%token EQ
%token NE
%token LT
%token LE
%token GT
%token GE
%token IF
%token PRINT


%type <num> expr

%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%%

math: math calc '\n'
	| calc '\n'
	;

calc:  expr					{ cout << "= " << $1 << "\n"; }
	| SQRT expr ')' 		{cout << sqrt($2); }
	| POW expr ',' expr ')'			{cout << pow($2, $4);}
	| ID '=' expr 			{ variables[$1] = $3; } 
	| IF expr ')'			{if($2 == true){cout << 1;}else{cout << 0;} }
	| PRINT expr ')'		{cout << $2; }
	; 

expr: expr '+' expr			{ $$ = $1 + $3; }
	| expr '-' expr   		{ $$ = $1 - $3; }
	| expr '*' expr			{ $$ = $1 * $3; }
	| expr '/' expr			
	{ 
		if ($3 == 0)
			yyerror("divisão por zero");
		else
			$$ = $1 / $3; 
	}
	| expr '<' expr { if($1 < $3){$$ = 1;}else{$$ = 0;} }
	| expr '>' expr { if($1 > $3){$$ = 1;}else{$$ = 0;} }
	| expr EQ expr { if($1 == $3){$$ = 1;}else{$$ = 0;} }
   	| expr NE expr { if($1 != $3){$$ = 1;}else{$$ = 0;} }
   	| expr LE expr { if($1 <= $3){$$ = 1;}else{$$ = 0;} }  	
   	| expr GE expr { if($1 >= $3){$$ = 1;}else{$$ = 0;} }
	| '(' expr ')'			{ $$ = $2; }
	| '-' expr %prec UMINUS { $$ = - $2; }
	| ID					{ $$ = variables[$1]; }
	| NUM
	;

%%

int main()
{
	yyparse();
}

void yyerror(const char * s)
{
	/* variáveis definidas no analisador léxico */
	extern int yylineno;    
	extern char * yytext;   

	/* mensagem de erro exibe o símbolo que causou erro e o número da linha */
    cout << "Erro (" << s << "): símbolo \"" << yytext << "\" (linha " << yylineno << ")\n";
}
