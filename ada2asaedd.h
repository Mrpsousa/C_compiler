#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int psize;
extern char ada[512];
extern unsigned long topoAda;
extern unsigned long indicesAda[512];
extern unsigned long topoIndicesAda;

char asa[512];
unsigned long topoAsa;
unsigned long indicesAsa[512];
unsigned long topoIndicesAsa;


void imprimirASA(char*, unsigned long*, unsigned long);
void pushAsa(char, int);
int indiceRealAda(int);
int A(int, int);
void quick_sort_asa(char*, unsigned long*, int, int);
int ada2asa();
