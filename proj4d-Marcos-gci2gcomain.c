#include "gci2gcoedd.h"

simbTabela tabelaSimbolos[100];
int qtdTabelaSimbolos;

int main(int argc, char *argv[])
{
    lerTabelaSimbolo();
	gci2gco(argv[1]);
}
