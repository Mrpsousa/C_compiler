#include "ada2asaedd.h"

int psize;
char ada[512];
unsigned long topoAda;
unsigned long indicesAda[512];
unsigned long topoIndicesAda;

int main(int argc, char *argv[])
{
	int i, j;
	psize = 12;

	topoAda = 0;
	topoIndicesAda = 0;

	char linha[16384];
	char parte[32];
	FILE *fp_ada;
	if((fp_ada = fopen(argv[1], "r")) == NULL){
		printf("Failed to run command\n");
		exit(1);
	}

	fscanf(fp_ada, " %[^\n]s", linha);

	i = 0;
	while(linha[i] != '\0'){
		j = 0;
		while(linha[i] != ' '){
			parte[j++] = linha[i++];
		}

		i++;
		parte[j] = '\0';

		indicesAda[topoIndicesAda++] = atol(parte);
	}

	fscanf(fp_ada, " %[^\n]s", linha);
	i = 0;
	j = 0;
	while(linha[i] != '\0'){
		ada[topoAda++] = linha[i];
		i+=2;
	}
	fclose(fp_ada);

	ada2asa();
}
