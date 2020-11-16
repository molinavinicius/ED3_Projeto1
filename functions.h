#ifndef FUNCTIONS_H   // guardas de cabeçalho, impedem inclusões cíclicas
#define FUNCTIONS_H

#define ERRO 1
#define OK 0
#define REG_SIZE 64
#define INDEX_SIZE 8
#define NAME_SIZE 40
#define TWITTER_SIZE 15

typedef struct{
    char status;
    int qtdPessoas;
}Header;

typedef struct{
    char removed;
    int idPessoa; 
    char name[40];
    int idade;
    char twitter[15];
}Registro;

typedef struct Index{
    struct Index* ant;
    int idPessoa; 
    int rrn;
    struct Index* prox; 
}Index;

typedef Index* Lista;

void printRegister( Registro registro);
int readRegister(FILE* fp, Registro *registro);
int alteraRegistro(FILE* fp, FILE* fi, int RRN, char* field, char* value);
int escreveRegistro(FILE* fp, Registro reg);
int func1(char* csv, char* file_bin, char* file_index);
int func2(char *filename);
int* func3(char *file_bin, char * file_index, char *field, char *value, int print);
int func4(char* file_bin, char* file_index, int n);
int func5(char* file_bin, char* file_index, int n);

#endif