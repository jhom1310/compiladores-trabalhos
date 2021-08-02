#include <iostream>
#include <fstream>
#include <cstring>
#include "parser.h"
#include "error.h"
using namespace std;

// arquivo de entrada
ifstream fin;

// programa pode receber nomes de arquivos
int main(int argc, char **argv)
{
	if (argc == 2 && strcmp(argv[1], "--help"))
	{
		 //fin.open("teste.prog",ios::in);
		fin.open(argv[1]);
		if (!fin.is_open())
		{
			cout << "Falha na abertura do arquivo \'" << argv[1] << "\'.\n";
			cout << "Verifique se o nome está correto, ou se é protegido para leitura.\n";
			exit(EXIT_FAILURE);
		}

		Parser tradutor;
		try
		{
			tradutor.Start();
		}
		catch (SyntaxError err)
		{
			err.What();
		}

		fin.close();
	}	
	else
	{
		cout << "Uso: tradutor arquivo\n";
		cout << "\nO arquivo deve ser como no exemplo abaixo:\n\n";
		cout << "math\n";
		cout << "{\n";
		cout << "   declarações\n";
		cout << "   instruções\n";
		cout << "}\n\n";
	}
}

