#include "np2gciedd.h"

#if defined(_WIN32) || defined(_WIN64)
	const char *del = "del *.inst";
#else
	#ifdef __linux
		const char *del = "rm *.inst";
		#else
		const char *del = "Unknown";
	#endif
#endif

char read()
{
	return polonesaR[count++];
}

void write(char *cont, int nameCount)
{
	FILE *arqSaida;
	char name[50];

	sprintf(arquivos[arquivosPoint++], "%d.inst", nameCount);
	sprintf(name, "%d.inst", nameCount);
	arqSaida = fopen(name, "w");
	fprintf(arqSaida, "%s", cont);
	fclose(arqSaida);


}

int isOperador(char c)
{
	if (c == '+' || c == '*' || c == '-' || c == '/' || c == '~')
	{
		return 1;
	}
	return 0;
}

int isOperadorLogicoBinario(char c)
{
	if (c == '>' || c == '<' || c == '&' || c == '|' || c == ':' || c == '_')
	{
		return 1;
	}
	return 0;
}

void geraPseudocodigo()
{
	FILE *files, *fp_gci;
	char line[50];
	fp_gci = fopen("proj4d-Marcos-GCI.txt", "w");

	for (int i = 0; i < arquivosPoint; i++)
	{
		files = fopen(arquivos[i], "r");
		if (files == NULL)
		{
			printf("Failed to run command\n");
			exit(1);
		}
		fscanf(files, "%[^\n]s", line);
		fprintf(fp_gci, "%s\n", line);
		fclose(files);
	}
	fprintf(fp_gci, "\n");
	fclose(fp_gci);
}

int expressao(int inst, int nameCount)
{
    int cabecalhoOn = 0;

	*token = read();
    if(polonesaR[count] != ';'){
        write("EXPR:", nameCount + inst);
        inst++;
        cabecalhoOn = 1;
    }
	while(*token != ';' && *token != '.'){
		if(*token == '+'){
			write("ADD", nameCount+inst);
			inst++;
		}
		else if(*token == '-'){
			write("SUB", nameCount+inst);
			inst++;
		}
		else if(*token == '*'){
			write("MUL", nameCount+inst);
			inst++;
		}
		else if(*token == '/'){
			write("DIV", nameCount+inst);
			inst++;
		}
		else if(*token == '~'){
			write("NEG", nameCount+inst);
			inst++;
		}
		else{
			write(token, nameCount + inst);
			inst++;
			if(polonesaR[count] != ';'){
				sprintf(aux, "push(%c)", *token);
				write(aux, nameCount + inst);
				inst++;
			}
		}
		*token = read();
	}

    if(cabecalhoOn){
        write("ENDEXPR:", nameCount + inst);
        inst++;
    }

	return inst;
}

int expressaoLogica(int inst, int nameCount)
{
    if(isOperadorLogicoBinario(*token)){
        sprintf(aux, "%c%c%c", polonesaR[count-1], *token, polonesaR[++count]);
        write(aux, nameCount+inst);
        inst++;
        *token = read();
    }
	else{
		sprintf(aux, "%c", *token);
        write(aux, nameCount+inst);
        inst++;
	}
    *token = read();

	return inst;
}

int leListaDeclaracoes(int inst, int nameCount){
    *token = read();
    while (*token == 'i' || *token == 'c')
    {
        if (*token == 'i')
        {
            *token = read();
            while (*token != ';')
            {
                write("TABSIMB.addInt(", nameCount + inst);
                inst++;
                write(token, nameCount + inst);
                inst++;
                write(")", nameCount + inst);
                inst++;
                *token = read();
            }
        }
        else
        {
            *token = read();
            while (*token != ';')
            {
                write("TABSIMB.addChar(", nameCount + inst);
                inst++;
                write(token, nameCount + inst);
                inst++;
                write(")", nameCount + inst);
                inst++;
                *token = read();
            }
        }
        //Lê o ;
        *token = read();

    }
    return inst;
}

int leAtribuicao(int inst, int nameCount){
    write("TABSIMB.update(", nameCount + inst);
    inst++;
    *token = read();
    sprintf(aux, "%c,", *token);
    write(aux, nameCount + inst);
    inst++;
    inst = expressao(inst, nameCount);
    write(")", nameCount + inst);
    inst++;
    *token = read();

    return inst;
}

int leListaAtribuicoes(int inst, int nameCount){
    while(*token == '='){
        *token = read();
        write("TABSIMB.update(", nameCount + inst);
        inst++;
        sprintf(aux, "%c,", *token);
        write(aux, nameCount + inst);
        inst++;
        inst = expressao(inst, nameCount);
        write(")", nameCount + inst);
		inst++;
        *token = read();
    }

    return inst;
}

int controle(int nameCount, int inst)
{
    char aux2[50];
	if (*token != '=')
	{
		inst = 0;
		nameCount = nameCount * 100 + 1000;
	}

	while(*token != '\0'){

		if (*token == 'f')
		{
			write("IF:", nameCount + inst);
			inst++;
			*token = read();
			inst = expressaoLogica(inst, nameCount);
            inst++;
			sprintf(aux2, "if(!%s) goto ENDIF", aux);
			write(aux2, nameCount + inst);
			inst++;

			inst = controle(nameCount, inst);

			write("ENDIF:", nameCount + 99);

            //Verificando IF's sequenciais
            *token = read();
            while(*token == 'f' || *token == 'w' || *token == 'o'){
                inst = controle(nameCount, inst);
            }
		}
		else if (*token == 'w')
		{
			write("WHILE:", nameCount + inst);
			inst++;
			*token = read();
			inst = expressaoLogica(inst, nameCount);
            inst++;
			sprintf(aux2, "if(!%s) goto ENDWHILE", aux);
			write(aux2, nameCount + inst);
			inst++;

			inst = controle(nameCount, inst);

			sprintf(aux, "goto WHILE:");
			write(aux, nameCount + inst);
			inst++;
			write("ENDWHILE:", nameCount + 98);
		}
		else if (*token == 'o')
		{
			*token = read();

			inst = leAtribuicao(inst, nameCount);
			
			write("FOR:", nameCount + inst);
			inst++;

    		inst = expressaoLogica(inst, nameCount);

			sprintf(aux2, "if(!%s) goto ENDFOR", aux);
			write(aux2, nameCount + inst);
			inst++;

			*token = read();


			inst = controle(nameCount, inst);

			write("goto FOR", nameCount + inst);
			inst++;
			write("ENDFOR:", nameCount + 99);
		}
		else if (*token == '=')
		{
			inst = leListaAtribuicoes(inst, nameCount);
            if(*token == '.'){
                return inst;
            }
		}
		else{
			break;
		}

		*token = read();
	}

	return inst;
}

int np2gci(){
	int nameCount = 1000;
	int inst = 0;
	char *endFunction;
	char aux2[2];

	arquivosPoint = 0;
	count = 0;
	inst = 0;
	nameCount = 1000;

    //Lendo tipo de retorno
    *token = read();
    //Lendo função
    *token = read();

    if(*token == 'm'){
		write("MAIN:", nameCount);

		nameCount = nameCount * 100 + nameCount;

		inst = leListaDeclaracoes(inst, nameCount);
		inst = leListaAtribuicoes(inst, nameCount);
		inst = controle(nameCount, inst);

		if(*token == 'r'){
			write("RETURN(", nameCount + inst);
			inst++;
			*token = read();
			sprintf(aux, "%c", *token);
			write(aux, nameCount + inst);
			inst++;
			write(")", nameCount + inst);
		}
    }

	write("END", nameCount + 99);
	geraPseudocodigo();
	system(del);
}
