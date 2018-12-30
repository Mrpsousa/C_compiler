#include "ada2asaedd.h"

void imprimirASA(char *arvore, unsigned long *indices, unsigned long topo){
	FILE *fp_tree;
	fp_tree = fopen("proj4d-Marcos-ASA.txt", "w");

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

void pushAsa(char c, int n){
	asa[topoAsa++] = c;
	indicesAsa[topoIndicesAsa++] = n;
}

int indiceRealAda(int valor){
	int inf, sup, meio;
	inf = 0;
	sup = topoIndicesAda-1;
	while (inf <= sup)
	{
		meio = (inf + sup)/2;
		if (valor == indicesAda[meio])
			return meio;
		else if (valor < indicesAda[meio])
			sup = meio-1;
		else
	        inf = meio+1;
	}
	return -1;
}

int A(int n, int p){
	int k = 0, v[psize];

	if (ada[indiceRealAda(p)] == 'i' && ada[indiceRealAda(p+1)] == 'm'){
		pushAsa(ada[indiceRealAda(p)], n);
		pushAsa(ada[indiceRealAda(p+1)], 3*(n)+1);
		A(3*(n)+2, (psize*(p+5))+1);
	}
	else if (ada[indiceRealAda(p)] == 'c' || ada[indiceRealAda(p)] == 'i'){
		pushAsa(ada[indiceRealAda(p)], n);
		pushAsa(ada[indiceRealAda(p+2)], n+1);
		A(3*(n)+1, (psize*(p+1))+1);
		A(3*(n+1)+1, (psize*(p+3))+1);
	}
	else if (ada[indiceRealAda(p+1)] == '=' && indiceRealAda(p+3) == -1){
		pushAsa(ada[indiceRealAda(p+1)], n);

		A(3*(n)+1, (psize*(p))+1);
		A(3*(n)+2, (psize*(p+2))+1);
	}
	else if (ada[indiceRealAda(p+1)] == '='){
		pushAsa(ada[indiceRealAda(p+1)], n);
		pushAsa(ada[indiceRealAda(p+3)], n+1);

		A(3*(n)+1, (psize*(p))+1);

		A(3*(n)+2, (psize*(p+2))+1);
		A(3*(n+1)+1, (psize*(p+4))+1);
	}
	else if (ada[indiceRealAda(p)] == '('){
		A(3*(n)+1, (psize*(p+1))+1);
		A(n, (psize*(p+2))+1);
		A(3*(n)+2, (psize*(p+3))+1);
	}
	else if (ada[indiceRealAda(p)] == 'r'){
		pushAsa(ada[indiceRealAda(p)], n);
		pushAsa(ada[indiceRealAda(p+4)], n+1);
		A(3*(n)+1, (psize*(p+2))+1);
	}
	else if (ada[indiceRealAda(p)] == 'f'){
		pushAsa(ada[indiceRealAda(p)], n);
		pushAsa('.', n+1);
		A(3*(n)+1, (psize*(p+2))+1);
		A(3*(n)+2, (psize*(p+5))+1);
		A(3*(n+1)+1, (psize*(p+7))+1);
	}
	else if (ada[indiceRealAda(p)] == 'w'){
		pushAsa(ada[indiceRealAda(p)], n);
		pushAsa('.', n+1);
		A(3*(n)+1, (psize*(p+2))+1);
		A(3*(n)+2, (psize*(p+5))+1);
		A(3*(n+1)+1, (psize*(p+7))+1);
	}
	else if (ada[indiceRealAda(p)] == 'o'){
		pushAsa(ada[indiceRealAda(p)], n);
		pushAsa('.', n+1);

		A(3*(n)+1, (psize*(p+2))+1);
		pushAsa(';', 3*(n)+2);
		A(3*(3*(n)+2)+1, (psize*(p+4))+1);
		pushAsa(';', 3*(3*(n)+2)+2);
		A(3*(3*(3*(n)+2)+2)+1, (psize*(p+9))+1);
		A(3*(n)+3, (psize*(p+6))+1);

		A(3*(n+1)+1, (psize*(p+11))+1);
	}
	else if (ada[indiceRealAda(p)] == '.'){
	}
	else if (ada[indiceRealAda(p)] == 'X'){
		A(n, (psize*(p))+1);
	}
	else{
		pushAsa(ada[indiceRealAda(p)], n);
	}

}

void quick_sort_asa(char *arvore, unsigned long *mapa, int left, int right) {
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
        quick_sort_asa(arvore, mapa, left, j);
    }
    if(i < right) {
        quick_sort_asa(arvore, mapa, i, right);
    }
}

int ada2asa(){
	topoAsa = 0;
	topoIndicesAsa = 0;
	A(0,1);
	quick_sort_asa(asa, indicesAsa, 0, topoIndicesAsa-1);
	imprimirASA(asa, indicesAsa, topoIndicesAsa);
}
