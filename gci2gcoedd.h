#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum fct
{
    LIT,
    OPR,
    LOD,
    STO,
    CAL,
    INT,
    JMP,
    JPC
} fct;

typedef struct instruction
{
    fct f;
    int l;
    int a;
} instruction;

#ifndef STRUCT_TABSIMB
#define STRUCT_TABSIMB
typedef struct{
	char tipo;
	char simbolo;
	char valor;
} simbTabela;
#endif

#define stacksize 500
instruction code[stacksize];
int cont;

extern simbTabela tabelaSimbolos[100];
extern int qtdTabelaSimbolos;

void read_inst(char *);
void setInstruction(fct, int, int);
int obterPosicaoTS(char);
int expression();
int control();
void lerTabelaSimbolo();
int gci2gco(char *);
