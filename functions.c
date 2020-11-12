#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <strings.h>

#include"functions.h"


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

void printRegister( Registro registro){
    printf("Dados da pessoa de codigo %d\n", registro.idPessoa);
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
    fread(&registro->removed, sizeof(char), 1, fp);
    if (registro->removed == '0'){
        return -1;
    }else{
        fread(&registro->idPessoa, sizeof(int), 1, fp);
        fread(registro->name, sizeof(char), 40, fp);
        fread(&registro->idade, sizeof(int), 1, fp);
        fread(registro->twitter, sizeof(char), 15, fp);
    return 0;
    }
}

FILE* openfile(char* filename, char* mode){
    char *path = (char *)malloc((strlen(filename)+16)*sizeof(char));
    strcpy(path, "casos-de-teste/");
    strcat(path, filename); 

    FILE* file = fopen(path, mode);
    if(file == NULL){
        printf("Falha no processamento do arquivo.");
        return NULL;
    }
    free(path);
    return file;
}

void changeStatus(FILE* fp, char status){
    int position = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    fwrite(&status, sizeof(char), 1, fp);
    fseek(fp, position, SEEK_SET);
    return;
}

int func1(char* csv){
    printf("oi");
    Registro* registros = (Registro *)calloc(10, sizeof(Registro*)); 
    printf("oi1");
    FILE* csvFile = openfile(csv, "rb");
    printf("oi2");
    char row[100];
    char buffer[40];
    char twitter[15];
    int id;
    int age;

    int i = 0;
    Registro reg;
    fscanf(csvFile,"%*[^\r\n]%*c");
    while( fgets (row, 100, csvFile)!=NULL ) {
        printf("%d",i);
        strcpy(buffer,"");
        int i = sscanf(row, "%d,%[^,],%d,%[^\n]", &reg.idPessoa, buffer,&reg.idade, reg.twitter);
        strcpy(reg.name, buffer);
        if (i < 4 ){
            sscanf(row, "%d,,%d,%s\n", &reg.idPessoa,&reg.idade,reg.twitter);
        }
        registros[i] = reg;
        i++;
        if(i%9==0){
            registros = realloc(registros, 2*i*sizeof(Registro));
        }
    }
    printf("%lu", sizeof(registros));

    return 0;
}

int func2(char* filename){

    Header header;
    Registro registro;    

    FILE *fp = openfile(filename, "rb");

    fread(&header.status, sizeof(char), 1, fp);
    fread(&header.qtdPessoas, sizeof(int), 1, fp);
    fseek(fp, 59, SEEK_CUR);

    if(header.qtdPessoas == 0){
        printf("Registro inexistente.");
    }
    else{
        for(int i = 0; i<header.qtdPessoas; i++){
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

int func3(char *file_bin, char * file_index, char *field, char *value){

  Header header;
  Registro registro;    

  FILE *fPessoa = openfile(file_bin, "rb");
  FILE *fIndex = openfile(file_index, "rb");

  fread(&header.status, sizeof(char), 1, fPessoa);
  fread(&header.qtdPessoas, sizeof(int), 1, fPessoa);
  fseek(fPessoa, 59, SEEK_CUR);

  if(header.qtdPessoas == 0){
    printf("Registro inexistente.");
    return -1;
  }
  int *result = (int*)malloc(sizeof(int));
  int r = 0;
  //BUSCA PELO ID
  if(strcmp(field, "idPessoa")==0){
    int id = atoi(value);
    int RRN = buscaBin(fIndex, id);
    if (RRN == -1){
        printf("Não encontrei");
    }
    else{
        fseek(fPessoa, 64*(RRN+1),SEEK_SET);
        if(readRegister(fPessoa, &registro)!=-1){
            printRegister(registro);
            result[r] = RRN;
            int* temp = 
        }else{
            printf("Registro removido.");
        } 
    }
  }
  //BUSCA PELA IDADE
  else if(strcmp(field, "idadePessoa")==0){
    int idade = atoi(value);
    for(int i = 0; i<header.qtdPessoas; i++){
        if(readRegister(fPessoa, &registro)!=-1){
          if(registro.idade == idade){
            printRegister(registro);
          }
        }else{
            fseek(fPessoa, 63, SEEK_CUR);
            continue;
        } 
    }
  }
  //BUSCA PELO NOME
  else if(strcmp(field, "nomePessoa")==0){

    for(int i = 0; i<header.qtdPessoas; i++){
        if(readRegister(fPessoa, &registro)!=-1){
          if(strcmp(registro.name, value) == 0){
            printRegister(registro);
          }
        }else{
            fseek(fPessoa, 63, SEEK_CUR);
            continue;
        } 
    }
  }

  //BUSCA PELO Twitter
  else if(strcmp(field, "twitterPessoa")==0){

    for(int i = 0; i<header.qtdPessoas; i++){
        if(readRegister(fPessoa, &registro)!=-1){
          if(strcmp(registro.twitter, value) == 0){
            printRegister(registro);
          }
        }else{
            fseek(fPessoa, 63, SEEK_CUR);
            continue;
        } 
    }
  }

  return 0;
}

