#include "gci2gcoedd.h"

char * instructionString[] = { "LIT", "OPR", "LOD", "STO", "CAL", "INT", "JMP", "JPC", "WRT" };
FILE *fp_gci;

void read_inst(char * instrucao){
	fscanf(fp_gci, " %[^\n]s", instrucao);
}

void setInstruction(fct func, int level, int arg){
	code[cont].f = func;
	code[cont].l = level;
	code[cont++].a = arg;

}

int obterPosicaoTS(char simb){
	for(size_t i = 0; i < qtdTabelaSimbolos; i++)
	{
		if(tabelaSimbolos[i].simbolo == simb){
			if(tabelaSimbolos[i].tipo == 's')
				return -1;
			return i + 3;
		}
	}
}

int expression(){
	int posicao;
	char instrucao[32];
	char aux[5], aux2[5];

	read_inst(instrucao);
	if(strcmp(instrucao, "EXPR:") == 0){
		read_inst(instrucao);
		while (strcmp(instrucao, "ENDEXPR:") != 0)
		{
			if(instrucao[0] == 'p'){
				posicao = obterPosicaoTS(aux[0]);
				if(posicao == -1){
					sprintf(aux2, "%c", aux[0]);
					setInstruction(LIT, 0, atoi(aux2));
				}
				else
					setInstruction(LOD, 0, posicao);
			}
			else if(strcmp(instrucao, "NEG") == 0){
				setInstruction(OPR, 0, 1);
			}
			else if(strcmp(instrucao, "ADD") == 0){
				setInstruction(OPR, 0, 2);
			}
			else if(strcmp(instrucao, "SUB") == 0){
				setInstruction(OPR, 0, 3);
			}
			else if(strcmp(instrucao, "MUL") == 0){
				setInstruction(OPR, 0, 4);
			}
			else if(strcmp(instrucao, "DIV") == 0){
				setInstruction(OPR, 0, 5);
			}
			else if(strcmp(instrucao, "IGUAL") == 0){
				setInstruction(OPR, 0, 8);
			}
			else if(strcmp(instrucao, "NAOIGUAL") == 0){
				setInstruction(OPR, 0, 9);
			}
			else if(strcmp(instrucao, "<") == 0){
				setInstruction(OPR, 0, 10);
			}
			else if(strcmp(instrucao, "MAIORIGUAL") == 0){
				setInstruction(OPR, 0, 11);
			}
			else if(strcmp(instrucao, ">") == 0){
				setInstruction(OPR, 0, 12);
			}
			else if(strcmp(instrucao, "MENORIGUAL") == 0){
				setInstruction(OPR, 0, 13);
			}
			else if(strcmp(instrucao, ".") == 0){
				continue;
			}
			else{
				strcpy(aux, instrucao);
			}
			read_inst(instrucao);
		}
	}
	else{
		if(instrucao[1] == '<' || instrucao[1] == '>'){
			posicao = obterPosicaoTS(instrucao[0]);
			if(posicao == -1){
				sprintf(aux2, "%c", instrucao[0]);
				setInstruction(LIT, 0, atoi(aux2));
			}
			else{
				setInstruction(LOD, 0, posicao);
			
			posicao = obterPosicaoTS(instrucao[2]);
			if(posicao == -1){
				sprintf(aux2, "%c", instrucao[2]);
				setInstruction(LIT, 0, atoi(aux2));
			}
			else
				setInstruction(LOD, 0, posicao);
			}

			if(instrucao[1] == '<')
				setInstruction(OPR, 0, 10);
			else
				setInstruction(OPR, 0, 12);
		}
		else{
			posicao = obterPosicaoTS(instrucao[0]);
			if(posicao == -1){
				sprintf(aux2, "%c", instrucao[0]);
				setInstruction(LIT, 0, atoi(aux2));
			}
			else
				setInstruction(LOD, 0, posicao);
		}
	}
}

int control(){
	int token;
	char instrucao[32];
	int pilha[32], indice = 0;

	while (fscanf(fp_gci, " %[^\n]s", instrucao) != EOF)
	{
		if(strcmp(instrucao, "MAIN:") == 0){
			setInstruction(INT, 0, 3);
		}
		else if(strcmp(instrucao, "END:") == 0){
			break;
		}
		else if(strcmp(instrucao, "TABSIMB.addChar(") == 0){
			setInstruction(INT, 0, 1);
		}
		else if(strcmp(instrucao, "TABSIMB.addInt(") == 0){
			setInstruction(INT, 0, 1);
		}
		else if(strcmp(instrucao, "TABSIMB.update(") == 0){
			read_inst(instrucao);
			expression();
			token = obterPosicaoTS(instrucao[0]);
			setInstruction(STO, 0, token);
			read_inst(instrucao);
		}
		else if(strcmp(instrucao, "IF:") == 0){
			expression();
			setInstruction(LIT, 0, 0);
			setInstruction(OPR, 0, 8);
			read_inst(instrucao);
			if(instrucao[0] == 'i'){
				setInstruction(JPC, 0, 0);
				pilha[indice++] = cont - 1;
			}
		}
		else if(strcmp(instrucao, "ENDIF:") == 0){
			code[pilha[--indice]].a = cont;
		}
		else if(strcmp(instrucao, "WHILE:") == 0){
			pilha[indice++] = cont;
			expression();
			setInstruction(LIT, 0, 0);
			setInstruction(OPR, 0, 8);
			read_inst(instrucao);
			if(instrucao[0] == 'i'){
				setInstruction(JPC, 0, 0);
				pilha[indice++] = cont - 1;
			}
		}
		else if(strcmp(instrucao, "ENDWHILE:") == 0){
			code[pilha[--indice]].a = cont + 1;
			setInstruction(JMP, 0, pilha[--indice]);
		}
		else if(strcmp(instrucao, "FOR:") == 0){
			pilha[indice++] = cont;
			expression();
			setInstruction(LIT, 0, 0);
			setInstruction(OPR, 0, 8);
			read_inst(instrucao);
			if(instrucao[0] == 'i'){
				setInstruction(JPC, 0, 0);
				pilha[indice++] = cont - 1;
			}
		}
		else if(strcmp(instrucao, "ENDFOR:") == 0){
			code[pilha[--indice]].a = cont + 1;
			setInstruction(JMP, 0, pilha[--indice]);
		}
		else{
			continue;
		}
	}
}

void lerTabelaSimbolo(){
	FILE * tabSimbFile;
	qtdTabelaSimbolos = 0;
	simbTabela newSimbolo;
	char linha[5], token[2];
	char a, b, c;
	tabSimbFile = fopen("proj4d-Marcos-tabsimb.txt", "r");
	if (!tabSimbFile) {
		return ;
	}

	while (!feof(tabSimbFile))
	{
		fscanf(tabSimbFile, " %[^\n]s", linha);
		newSimbolo.tipo = linha[0];
		newSimbolo.simbolo = linha[2];
		newSimbolo.valor = linha[4];

		tabelaSimbolos[qtdTabelaSimbolos++] = newSimbolo;
	}

	fclose(tabSimbFile);
}

int gci2gco(char *file_in){
    cont = 0;
    FILE *fp_gco;
    if((fp_gci = fopen(file_in, "r")) == NULL){
		return -1;
	}
    fp_gco = fopen("proj4d-Marcos-GCO.txt", "w");

	control();
	setInstruction(OPR, 0, 0);

	for(size_t i = 0; i < cont; i++)
	{
		fprintf(fp_gco, "%s %d %d\n", instructionString[code[i].f], code[i].l, code[i].a);
	}
	
    fclose(fp_gci);
    fclose(fp_gco);
}
