#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <strings.h>

#include"functions.h"
#include "aux_func.h"


void printRegister( Registro registro){
    printf("Dados da pessoa de código %d\n", registro.idPessoa);
    if (strlen(registro.name)){
        printf("Nome: %s\n", registro.name);
    }else{
        printf("Nome: -\n");
    }
    if (registro.idade == -1){
        printf("Idade: -\n");
    }else{
        printf("Idade: %d anos\n", registro.idade);
    }
    printf("Twitter: %s\n", registro.twitter);
    printf("\n");
}

int readRegister(FILE* fp, Registro *registro){
    int RRN = ftell(fp);
    fread(&registro->removed, sizeof(char), 1, fp);
    if (registro->removed == '0'){
        return -1;
    }else{
        fread(&registro->idPessoa, sizeof(int), 1, fp);
        fread(registro->name, sizeof(char), 40, fp);
        fread(&registro->idade, sizeof(int), 1, fp);
        fread(registro->twitter, sizeof(char), 15, fp);
    return RRN/REG_SIZE;
    }
}

void changeStatus(FILE* fp, char status){
    int position = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    fwrite(&status, sizeof(char), 1, fp);
    fseek(fp, position, SEEK_SET);
    return;
}

//TODO: adicionar os caracteres de lixo '$'
int alteraRegistro(FILE* fp, FILE* fi, int RRN, char* field, char* value){
    if(strcmp(field, "idPessoa")==0){
        int id = atoi(value);
        int pos = buscaBin(fi, id);
        if (pos == -1){
            printf("Registro inexistente.");
        }
        //Atualizar o arquivo .index
        fseek(fi, (pos+1)*INDEX_SIZE, SEEK_SET);
        fwrite(&id, sizeof(int), 1, fi);

        //Atualizar o arquivo .bin
        fseek(fp, RRN*REG_SIZE + 1, SEEK_SET);//aqui o +1 é para pular o campo de removido
        fwrite(&id, sizeof(int), 1, fp);

    }else if(strcmp(field, "nomePessoa")==0){

        fseek(fp, RRN*REG_SIZE + 5, SEEK_SET);
        writeFilled(fp, value, NAME_SIZE, 0);

    }else if(strcmp(field, "idadePessoa")==0){
        int age = atoi(value);

        fseek(fp, RRN*REG_SIZE + 45, SEEK_SET);
        fwrite(&age, sizeof(int), 1, fp);

    }else if(strcmp(field, "twitterPessoa")==0){
        fseek(fp, RRN*REG_SIZE + 49, SEEK_SET);
        writeFilled(fp, value, TWITTER_SIZE, 0);
    }
    return OK;
}

int escreveRegistro(FILE* fp, Registro reg){
    int rrn = ftell(fp)/REG_SIZE;
    fwrite(&reg.removed,sizeof(char), 1, fp);
    fwrite(&reg.idPessoa, sizeof(int),1,fp);
    writeFilled(fp,reg.name,NAME_SIZE,0);
    fwrite(&reg.idade, sizeof(int),1,fp);
	trim(reg.twitter);
    writeFilled(fp, reg.twitter, TWITTER_SIZE,0);
    return rrn;
}


int func1(char* csv, char* file_bin, char* file_index){

    FILE* csvFile = openfile(csv, "rb");
    if(csvFile == NULL){
        printf("Falha no carregamento do arquivo.");
        return ERRO;
    }

    FILE* fPessoa = openfile(file_bin, "wb+");
    if(fPessoa == NULL){
        printf("Falha no processamento do arquivo.");
        return ERRO;
    }

    FILE* fIndex = openfile(file_index, "wb+");
    if(fIndex == NULL){
        printf("Falha no processamento do arquivo.");
        return ERRO;
    }

    //ESCREVE O CABEÇALHO------------
    int qtdPessoas = 0;
    char statusBin = '0';
    char statusIndex = '0';
    char lixo[59];
    strcpy(lixo,"");

    fwrite(&statusBin,sizeof(char),1,fPessoa);
    fwrite(&qtdPessoas,sizeof(int),1,fPessoa);
    writeFilled(fPessoa, lixo, 59, 1);

    strcpy(lixo,"");
    fwrite(&statusIndex,1,1,fIndex);
    writeFilled(fIndex, lixo, 7, 1);
    //---------------------------------

    Lista* list = criaLista();  

    Index ind;

    char row[100];
    char buffer[100];

    Registro reg;
    fgets (row, 100, csvFile);
    fflush(stdin);
    while( fgets (row, 100, csvFile)!=NULL ) {
		reg.idPessoa = -1;
		strcpy(reg.twitter, "");

        strcpy(buffer,"");
        int i = sscanf(row, "%d,%[^,],%d,%[^\n]", &reg.idPessoa, buffer,&reg.idade, reg.twitter);
        if(strlen(buffer)>=40){
            buffer[39] = '\0';
        }
        strcpy(reg.name, buffer);
        if (i < 4 ){
            sscanf(row, "%d,,%d,%s\n", &reg.idPessoa,&reg.idade,reg.twitter);
        }

        reg.removed = '1';

		if((reg.idPessoa == -1) || strcmp(reg.twitter,"")==0){
			continue;
		}

        ind.idPessoa = reg.idPessoa;
        ind.rrn = escreveRegistro(fPessoa, reg) - 1;
        insereListaOrdenado(list, ind);
        qtdPessoas++;
    }
    escreveLista(fIndex, list);
    fseek(fPessoa,1,SEEK_SET);
    fwrite(&qtdPessoas,sizeof(int),1,fPessoa);

	changeStatus(fPessoa, '1');
	changeStatus(fIndex, '1');

    fclose(fPessoa);
    fclose(fIndex);
    liberaLista(list);

    binarioNaTela1(file_bin, file_index);

    return 0;
}

int func2(char* filename){

    Header header;
    Registro registro;    

    FILE *fp = openfile(filename, "rb");
    if(fp == NULL){
        printf("Falha no processamento do arquivo.");
        return ERRO;
    }

    fread(&header.status, sizeof(char), 1, fp);
    if(header.status == '0'){
        printf("Falha no processamento do arquivo.");
        return ERRO;
    }
    fread(&header.qtdPessoas, sizeof(int), 1, fp);
    fseek(fp, 59, SEEK_CUR);

    if(header.qtdPessoas == 0){
        printf("Registro inexistente.");
    }
    else{
        for(int i = 0; i<=header.qtdPessoas; i++){
            if(readRegister(fp, &registro)!=-1){
                printRegister(registro);
            }else{
                fseek(fp, 63, SEEK_CUR);
                continue;
            } 
        }
    }
    fclose(fp);
    return 0;
}

int* func3(char *file_bin, char * file_index, char *field, char *value, int print){
   /*
   Description: 

   Argument:
        file_bin    -- arquivo .bin
        file_index  -- arquivo .index
        field       -- campo como target de busca
        value       -- valor a ser encontrado no field

   Returns: 
        int* result -- array de inteiros contendo os RRNs dos registros. 
            result[0] -- é a quantidade de registros retornados.
   */

  Header header;
  Registro registro;    

  FILE *fPessoa = openfile(file_bin, "rb");
  if(fPessoa == NULL){
        printf("Falha no processamento do arquivo.");
        exit(0);
    }
  FILE *fIndex = openfile(file_index, "rb");
  if(fIndex == NULL){
        printf("Falha no processamento do arquivo.");
        exit(0);
    }

  fread(&header.status, sizeof(char), 1, fPessoa);
  char statusIndex;
  fread(&statusIndex, sizeof(char), 1, fIndex);
  if(header.status == '0' || statusIndex == '0'){
        printf("Falha no processamento do arquivo.");
        exit(0);
    }
  fread(&header.qtdPessoas, sizeof(int), 1, fPessoa);
  fseek(fPessoa, 59, SEEK_CUR);

  if(header.qtdPessoas == 0){
    printf("Registro inexistente.");
    exit(0);
  }

  int *result = (int*)malloc(sizeof(int));
    if (result == NULL){
        printf("[FUNC3] Erro na alocação.");
        exit(0);
    }
  int r = 1;

  //BUSCA PELO ID
  //TODO: verificar se o RRN em idPessoa está correto
  if(strcmp(field, "idPessoa")==0){
    int id = atoi(value);
    int RRN = buscaBin(fIndex, id);
    if (RRN == -1){
        printf("Registro inexistente.");
    }
    else{
        fseek(fPessoa, 64*(RRN+1),SEEK_SET);
        RRN = readRegister(fPessoa, &registro);
        if(RRN != -1){
            if (print)
            printRegister(registro);
            result[r] = RRN;
            int* temp = realloc(result, (r++)*sizeof(int));
            if (temp == NULL){
                printf("[FUNC3] Erro na realocação.");
                exit(0);
            }
            result = temp;
        }else{
            printf("Registro removido.");
        } 
    }
  }
  //BUSCA PELA IDADE
  else if(strcmp(field, "idadePessoa")==0){
    int idade = atoi(value);
    int RRN;
    for(int i = 0; i<header.qtdPessoas; i++){
        RRN = readRegister(fPessoa, &registro);
        if(RRN != -1){
          if(registro.idade == idade){
            printRegister(registro);
            result[r] = RRN;
            int* temp = realloc(result, (r+2)*sizeof(int));
            if (temp == NULL){
                printf("[FUNC3] Erro na realocação.");
                exit(0);
            }
            result = temp;
            r++;
          }
        }else{
            fseek(fPessoa, 63, SEEK_CUR);
            continue;
        } 
       
    }
  }
  //BUSCA PELO NOME
  else if(strcmp(field, "nomePessoa")==0){
    int RRN;
    for(int i = 0; i<header.qtdPessoas; i++){
        RRN = readRegister(fPessoa, &registro);
        if(RRN != -1){
          if(strcmp(registro.name, value) == 0){
            printRegister(registro);
            result[r] = RRN;
            int* temp = realloc(result, (r+2)*sizeof(int));
            if (temp == NULL){
                printf("[FUNC3] Erro na realocação.");
                exit(0);
            }
            result = temp;
            r++;
          }
        }else{
            fseek(fPessoa, 63, SEEK_CUR);
            continue;
        } 
    }
  }

  //BUSCA PELO Twitter
  else if(strcmp(field, "twitterPessoa")==0){
    int RRN;
    for(int i = 0; i<header.qtdPessoas; i++){
        RRN = readRegister(fPessoa, &registro);
        if(RRN != -1){
          if(strcmp(registro.twitter, value) == 0){
            printRegister(registro);
            result[r] = RRN;
            int* temp = realloc(result, (r+2)*sizeof(int));
            if (temp == NULL){
                printf("[FUNC3] Erro na realocação.");
                exit(0);
            }
            result = temp;
            r++;
          }
        }else{
            fseek(fPessoa, 63, SEEK_CUR);
            continue;
        } 
    }
  }
  result[0] = r-1;
  return result;
}

int func4(char* file_bin, char* file_index, int n){
    Header header;
    Registro registro; 
    Index index;   

    FILE *fPessoa = openfile(file_bin, "r+b");
    if(fPessoa == NULL){
        printf("Falha no processamento do arquivo.");
        exit(0);
    }
    FILE *fIndex = openfile(file_index, "w");
    if(fPessoa == NULL){
        printf("Falha no processamento do arquivo.");
        exit(0);
    }

    Lista* list = read2Lista(fIndex);
    fread(&header.status, sizeof(char), 1, fPessoa);
    fread(&header.qtdPessoas, sizeof(int), 1, fPessoa);

    changeStatus(fPessoa, '0');
    changeStatus(fIndex,'0');

    fseek(fPessoa,0,SEEK_END);

    char buff[10];
    char buffer[100];

    for(int i=0; i<n;i++){

		//Lê o novo ID;
        scanf("%d", &registro.idPessoa);
		//Lê o nome;
        scan_quote_string(buffer);
		if(strlen(buffer)>=40){
            buffer[39] = '\0';
        }
        strcpy(registro.name, buffer);
		//Lê a idade
        scan_quote_string(buff);
        if(strcmp(buff, "") == 0){
            registro.idade = -1;
        }else{
            registro.idade = atoi(buff);
        }
        scan_quote_string(registro.twitter);

        index.idPessoa = registro.idPessoa;
        index.rrn = escreveRegistro(fPessoa, registro) - 1;
        insereListaOrdenado(list, index);
        header.qtdPessoas++;
    }
    
    fseek(fPessoa,1,SEEK_SET);
    fwrite(&header.qtdPessoas,sizeof(int),1,fPessoa);

    char lixo[20];
    strcpy(lixo,"");
    char statusIndex = '1';
    fwrite(&statusIndex,1,1,fIndex);
    writeFilled(fIndex, lixo, 7, 1);

    escreveLista(fIndex, list);

    fclose(fIndex);
    fclose(fPessoa);
    liberaLista(list);

    binarioNaTela1(file_bin, file_index);

    return OK;
}

int func5(char* file_bin, char* file_index, int n){

    Header header;
    Registro registro;    

    FILE *fPessoa = openfile(file_bin, "r+b");
    if(fPessoa == NULL){
        printf("Falha no processamento do arquivo.");
        exit(0);
    }
    FILE *fIndex = openfile(file_index, "r+b");
    if(fPessoa == NULL){
        printf("Falha no processamento do arquivo.");
        exit(0);
    }

    fread(&header.status, sizeof(char), 1, fPessoa);
    fread(&header.qtdPessoas, sizeof(int), 1, fPessoa);
    fseek(fPessoa, 59, SEEK_CUR);
    if(header.qtdPessoas == 0){
        printf("Registro inexistente.");
        exit(0);
    }

    char row[300];
    char remain[300];
    char field[25];
    char value[40];
    int m;

    changeStatus(fPessoa, '0');
    changeStatus(fIndex, '0');


    for(int i=0; i<n;i++){
        scanf("%s", field);
        scan_quote_string(value);
        scanf("%d", &m);
        int* RRN = func3(file_bin,file_index,field,value,0);
        int len = RRN[0];
        for(int j=0; j<m;j++){
            scanf("%s", field);
            scan_quote_string(value);
            for(int k =1; k<=len; k++){
                if (alteraRegistro(fPessoa, fIndex, RRN[k], field, value) == ERRO){
                    printf("Erro na alteração.");
                    exit(0);
                }
            }
        }        

    }

    changeStatus(fPessoa, '1');
    changeStatus(fIndex, '1');
    fclose(fPessoa);
    fclose(fIndex);
    binarioNaTela1(file_bin, file_index);
    return OK;
}