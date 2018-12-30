#include "parseredd.h"

char pop(){
	return pilha[topo--];
}

int isVariavel(char c){
	for(size_t k = 0; k < fimVariaveis; k++)
	{
		if(c == variaveis[k])
			return 1;
	}
	return 0;
}

void pushAda(produc p, int n){
	int tam = strlen(p.simbolos);
	for(size_t k = tam - 1; k != -1; k--)
	{
		pilha[++topo] = p.simbolos[k];
		if(isVariavel(p.simbolos[k]))
			nivel[tnivel++] = psize*n+k+1;
		ada[topoAda++] = p.simbolos[k];
		indicesAda[topoIndicesAda++] = psize*n+k+1;
	}
	if(fimArvore < psize*n+tam+1){
		fimArvore = psize*n+tam+1;
		}
}

int buscarProducao(char variavel, char token){
	for(size_t i = 0; i < qtdProducoes; i++){
		if(producoes[i].variavel[0] == variavel){
			if(isVariavel(producoes[i].simbolos[0])){
				int p = buscarProducao(producoes[i].simbolos[0], token);
				if(p != -1)
					return p;
			}
			else if(token == producoes[i].simbolos[0])
				return i;
		}
	}
	return -1;
}

void adicionarTabSimb(char tipo, char simb, char valor){
	simbTabela newSimbolo;
	newSimbolo.tipo = tipo;
	newSimbolo.simbolo = simb;
	newSimbolo.valor = valor;

	tabelaSimbolos[qtdTabelaSimbolos++] = newSimbolo;
}

void S(){
	int j = 0, n = 0, p = 1, flag = 0;
	pilha[topo] = variaveis[0];

	ada[topoAda++] = variaveis[0];
	indicesAda[topoIndicesAda++] = 0;

	nivel[tnivel++] = 0;
	while(cadeia[j] != '\0'){
		for(size_t i = 0; i < qtdProducoes; i++)
		{
			if(pilha[topo] == producoes[i].variavel[0]){
				if(isVariavel(producoes[i].simbolos[0])){
					if(buscarProducao(producoes[i].simbolos[0], cadeia[j]) != -1)
						flag = 1;
				}
				if(cadeia[j] == producoes[i].simbolos[0] || flag == 1){
					if((producoes[i].simbolos[0] == 'i' || producoes[i].simbolos[0] == 'c') && producoes[i].simbolos[1] != 'm'){
						adicionarTabSimb(cadeia[j], cadeia[j+1], '0');
					}
					flag = 0;
					pop();
					tnivel--;
					pushAda(producoes[i], nivel[tnivel]);
					n = psize*n + strlen(producoes[i].simbolos);
				}
			}
			else if(pilha[topo] == cadeia[j]){
				pop();
				j++;
				n++;
				break;
			}
		}
	}
}

void quick_sort_ada(char *arvore, unsigned long *mapa, int left, int right) {
    int i, j, x, y;
	char a;

    i = left;
    j = right;
    x = mapa[(left + right) / 2];

    while(i <= j) {
        while(mapa[i] < x && i < right) {
            i++;
        }
        while(mapa[j] > x && j > left) {
            j--;
        }
        if(i <= j) {
            y = mapa[i];
            mapa[i] = mapa[j];
            mapa[j] = y;
			a = arvore[i];
            arvore[i] = arvore[j];
            arvore[j] = a;
            i++;
            j--;
        }
    }

    if(j > left) {
        quick_sort_ada(arvore, mapa, left, j);
    }
    if(i < right) {
        quick_sort_ada(arvore, mapa, i, right);
    }
}

void imprimirADA(char *arvore, unsigned long *indices, unsigned long topo){
	FILE *fp_tree;
	fp_tree = fopen("proj4d-Marcos-ADA.txt", "w");

	for(size_t j = 0; j < topo; j++)
	{
		fprintf(fp_tree, "%u ", indices[j]);
	}
	fprintf(fp_tree, "\n");

	for(size_t j = 0; j < topo; j++)
	{
		fprintf(fp_tree, "%c ", arvore[j]);
	}
	fprintf(fp_tree, "\n\n");
	fclose(fp_tree);
}

int parser(char *file_in){
	int i, j;
	FILE * tabSimbFile;
	psize = 0;
	topoAda = 0;
	topoIndicesAda = 0;
	fimArvore = fimVariaveis = qtdProducoes = qtdTabelaSimbolos = 0;

	FILE *fp_glc, *fp_cadeia;
	if((fp_glc = fopen("proj4d-Marcos-GLC.txt", "r")) == NULL){
		return -1;
	}
	if((fp_cadeia = fopen(file_in, "r")) == NULL){
		return -1;
	}

	while (fscanf(fp_glc, " %[^\n]s", prod) != EOF)
	{
		i = 0;
		j = 0;
		produc p;
		if(fimVariaveis == 0 || variaveis[fimVariaveis - 1] != prod[i])
			variaveis[fimVariaveis++] = prod[i];

		p.variavel[j++] = prod[i++];
		p.variavel[j] = '\0';

		j = 0;
		while(prod[i] != '\0'){
			p.simbolos[j++] = prod[i++];
		}
		p.simbolos[j] = '\0';
		if(strlen(p.simbolos) > psize)
			psize = strlen(p.simbolos);

		producoes[qtdProducoes++] = p;
	}

	fscanf(fp_cadeia, " %[^\n]s", cadeia);
	topo = 0;
	tnivel = 0;

	S();
	quick_sort_ada(ada, indicesAda, 0, topoIndicesAda-1);
	imprimirADA(ada, indicesAda, topoIndicesAda);

	tabSimbFile = fopen("proj4d-Marcos-tabsimb.txt", "w");
	for(size_t i = 0; i < qtdTabelaSimbolos; i++)
	{
		fprintf(tabSimbFile, "%c %c %c\n", tabelaSimbolos[i].tipo, tabelaSimbolos[i].simbolo, tabelaSimbolos[i].valor);
	}
	for(size_t i = 0; i < 10; i++)
	{
		adicionarTabSimb('s', i+'0', i+'0');
		fprintf(tabSimbFile, "s %d %d\n", i, i);
	}

	fclose(tabSimbFile);
	fclose(fp_cadeia);
	fclose(fp_glc);
}
