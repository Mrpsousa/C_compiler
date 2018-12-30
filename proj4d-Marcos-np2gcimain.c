#include "np2gciedd.h"

char polonesaR[512];

int main(int argc, char *argv[])
{
	FILE *fp_np;
	if((fp_np = fopen(argv[1], "r")) == NULL){
		printf("Failed to run command\n");
		exit(1);
	}
	fscanf(fp_np, " %[^\n]s", polonesaR);
	fclose(fp_np);
	np2gci();

	return 0;
}
