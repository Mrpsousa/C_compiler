#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char polonesaR[512];
int iP;

extern char asa[512];
extern unsigned long topoAsa;
extern unsigned long indicesAsa[512];
extern unsigned long topoIndicesAsa;

int asa2np();
int indiceRealAsa(int);
void asaToPolonesaReversa(int);
void asaToPolonesa(int);
