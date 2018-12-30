#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count;
char token[1];
char aux[50];
char arquivos[200][50];
int arquivosPoint;

extern char polonesaR[512];

char read();
void write(char*, int);
int isOperador(char);
int isOperadorLogicoBinario(char);
void geraPseudocodigo();
int expressao(int, int);
int expressaoLogica(int, int);
int leListaDeclaracoes(int, int);
int leAtribuicao(int, int);
int leListaAtribuicoes(int, int);
int controle(int, int);
int np2gci();
