#include "asa2npedd.h"

int indiceRealAsa(int valor){
	int inf, sup, meio;
	inf = 0;
	sup = topoIndicesAsa-1;
	while (inf <= sup)
	{
		meio = (inf + sup)/2;
		if (valor == indicesAsa[meio])
			return meio;
		else if (valor < indicesAsa[meio])
			sup = meio-1;
		else
	        inf = meio+1;
	}
	return -1;
}

void asaToPolonesaReversa(int n){
	if(indiceRealAsa(3*n+1) == -1 || 3*n+1 > indicesAsa[topoIndicesAsa - 1]){
		polonesaR[iP++] = asa[indiceRealAsa(n)];
	}
	else{
		asaToPolonesaReversa(3*n+1);
		if(asa[indiceRealAsa(3*n+2)] != ' ' && 3*n+2 <= indicesAsa[topoIndicesAsa - 1]){
			asaToPolonesaReversa(3*n+2);
		}
		polonesaR[iP++] = asa[indiceRealAsa(n)];
	}
}

void asaToPolonesa(int n){
	if(indiceRealAsa(3*n+1) == -1 || 3*n+1 > indicesAsa[topoIndicesAsa - 1]){
		polonesaR[iP++] = asa[indiceRealAsa(n)];
	}
	else{
		polonesaR[iP++] = asa[indiceRealAsa(n)];
		asaToPolonesa(3*n+1);

		for(size_t k = 2; indiceRealAsa(3*n+k) != -1 && 3*n+k <= indicesAsa[topoIndicesAsa - 1] && k <= 3; k++)
		{
			if(asa[indiceRealAsa(n)] != '=')
				asaToPolonesa(3*n+k);
			else
				asaToPolonesaReversa(3*n+k);
		}
	}
}

int asa2np(){
	FILE *fp_np;
	fp_np = fopen("proj4d-Marcos-NP.txt", "w");

	asaToPolonesa(0);
	polonesaR[iP] = '\0';
	fprintf(fp_np, "%s\n", polonesaR);

	fclose(fp_np);
}
