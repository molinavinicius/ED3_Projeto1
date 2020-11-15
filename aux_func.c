#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <strings.h>
#include"functions.h"
#include "aux_func.h"


void remove_quotes(char * line){
    char *dst = line;
    char *src = line;
    char c;

    while ((c = *src++) != '\0')
    {
        if (c == '\\')
        {
            *dst++ = c;
            if ((c = *src++) == '\0')
                break;
            *dst++ = c;
        }
        else if (c != '"')
            *dst++ = c;
    }
    *dst = '\0';
}

void scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então...
		*str = R;
		str++;
		while((R = getchar()) != EOF && !isspace(R)) {
			*str = R;
			str++;
		}
		*str = '\0';
	} else { // EOF
		strcpy(str, "");
	}
}



FILE* openfile(char* filename, char* mode){
    char *path = (char *)malloc((strlen(filename)+16)*sizeof(char));
    strcpy(path, "casos-de-teste/");
    strcat(path, filename); 

    FILE* file = fopen(path, mode);

    free(path);
    return file;
}

int buscaBin(FILE* fp, int idPessoa){
    int left = 1;
    fseek(fp,0,SEEK_END);
    int right = ftell(fp)/8;
    int middle;
    
    Registro registro; 
    int id;
    int RRN; 
    while(left < right){

        middle = (left + right)/2;
        fseek(fp,8*middle, SEEK_SET);
        fread(&id, sizeof(int), 1,fp);
        fread(&RRN, sizeof(int), 1,fp);

        if (id == idPessoa){
            return RRN;
        }
        if (idPessoa > id){
            left = middle;
        }else{
            right = middle;
        }
    }
    return -1;    
}

void binarioNaTela1(char *nomeArquivoBinario, char *nomeArquivoIndice) {

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	FILE *fs, *fi;
	size_t fl;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO NA FUNÇÃO BINARIO NA TELA. Não se esqueça do fclose e verifique se passou o argumento corretamente!\n");
		return;
	}
	if(nomeArquivoIndice == NULL || !(fi = fopen(nomeArquivoIndice, "rb"))) {
		fprintf(stderr, "ERRO NA FUNÇÃO BINARIO NA TELA. Não se esqueça do fclose e verifique se passou o argumento corretamente!\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);
	fclose(fs);
	cs = fl;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	fseek(fi, 0, SEEK_END);
	fl = ftell(fi);
	fseek(fi, 0, SEEK_SET);
	mb = (unsigned char *) realloc(mb, fl);
	fread(mb, 1, fl, fi);
	fclose(fi);
	cs += fl;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
}
