%{

#include <iostream>
#include <fstream>
#include <string>
using namespace std;
// Utilizado estados para descartar conteudo do <syles> e uma variavel de apoio para identação.
unsigned charCount = 0, lineCount = 0, tabulacao = 0, pares = 0;

%}

%option noyywrap
%s STYLE

space [ ]+
eol \n
comment <!.*[-->|>]
img <img[^>|^<]*>
br <br> 
tagf <\/[^>|^<]*>
tag <[^>|^<]*>
txt [a-zA-z0-9!\._,\)\(\":\}\{\;\-\%&\+ ]*



%%
"<style>"	{for(int x=0;x<tabulacao;x++){
          cout << "|\t";
          }
    	  cout << "+--" << YYText() << endl; tabulacao += 1; pares += 1;
		  BEGIN STYLE;}

<STYLE>{txt}	{}

<STYLE>"</style>"	{BEGIN INITIAL; tabulacao -=1;
		for(int x=0;x<tabulacao;x++){
        cout << "|\t";
        }
    	cout << "+--" << YYText() << endl;}
{space} { }
{eol}	{ lineCount++; }
{comment}	{};

{tagf}	{tabulacao -=1;
		for(int x=0;x<tabulacao;x++){
        cout << "|\t";
        }
    	cout << "+--" << YYText() << endl;}

{img}	{for(int x=0;x<tabulacao;x++){
        cout << "|\t";
        }
    	cout << "+--" << YYText() << endl;  pares += 1;}

{br}	{for(int x=0;x<tabulacao;x++){
        cout << "|\t";
        }
    	cout << "+--" << YYText() << endl; pares += 1;}

{tag}	{for(int x=0;x<tabulacao;x++){
        cout << "|\t";
        }
    	cout << "+--" << YYText() << endl; tabulacao += 1; pares += 1;}

{txt}	{for(int x=0;x<tabulacao;x++){
        cout << "|\t";
        }
		cout << "Texto["<< YYLeng() << "]" << endl; charCount += YYLeng();}

.	    { }

%%

int main(int argc, char ** argv)
{
	yyFlexLexer lexer;  // lexer lê da entrada padrão (teclado)
	ifstream fin;
	
	if (argc > 1) 
	{    
		fin.open(argv[1]);
		if (!fin.is_open()) 
        {
			cout << "Arquivo " << argv[1] << " não pode ser aberto!\n";
			exit(EXIT_FAILURE);
		}
		else
		{
			// altera entrada para arquivo aberto
			lexer.switch_streams(&fin);
		}
	}

	lexer.yylex();
	
	cout << "Tags HTML: " << pares << "\n"
		 << "Linhas: " << lineCount << "\n"
		 << "Chars: " << charCount << "\n";

	fin.close();
	return 0;
}
