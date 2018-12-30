#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct producao{
	char variavel[5];
	char simbolos[20];
} produc;

#ifndef STRUCT_TABSIMB
#define STRUCT_TABSIMB
typedef struct{
	char tipo;
	char simbolo;
	char valor;
} simbTabela;
#endif

char prod[50];
char pilha[1024];
char cadeia[512];
int fimArvore;
int topo;
int psize;
int nivel[50];
int tnivel;

char ada[512];
unsigned long topoAda;
unsigned long indicesAda[512];
unsigned long topoIndicesAda;

char variaveis[50];
int fimVariaveis;

produc producoes[500];
int qtdProducoes;

simbTabela tabelaSimbolos[100];
int qtdTabelaSimbolos;

char pop();
int isVariavel(char);
void pushAda(produc, int);
int buscarProducao(char, char);
void adicionarTabSimb(char, char, char);
void S();
void quick_sort_ada(char *, unsigned long *, int, int);
void imprimirADA(char*, unsigned long *, unsigned long);
int parser();
