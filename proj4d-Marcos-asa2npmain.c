#include "asa2npedd.h"

char asa[512];
unsigned long topoAsa;
unsigned long indicesAsa[512];
unsigned long topoIndicesAsa;

int main(int argc, char *argv[]){
	int i, j;

	topoAsa = 0;
	topoIndicesAsa = 0;

	char linha[16384];
	char parte[32];
	FILE *fp_asa;
	if((fp_asa = fopen(argv[1], "r")) == NULL){
		printf("Failed to run command\n");
		exit(1);
	}
	fscanf(fp_asa, " %[^\n]s", linha);

	i = 0;
	while(linha[i] != '\0'){
		j = 0;
		while(linha[i] != ' '){
			parte[j++] = linha[i++];
		}

		i++;
		parte[j] = '\0';

		indicesAsa[topoIndicesAsa++] = atol(parte);
	}

	fscanf(fp_asa, " %[^\n]s", linha);
	i = 0;
	j = 0;
	while(linha[i] != '\0'){
		asa[topoAsa++] = linha[i];
		i+=2;
	}
	fclose(fp_asa);
	asa2np();
	return 0;
}
