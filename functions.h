#ifndef FUNCTIONS_H   // guardas de cabeçalho, impedem inclusões cíclicas
#define FUNCTIONS_H

#define ERRO 1
#define OK 0
#define REG_SIZE 64
#define INDEX_SIZE 8


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

typedef struct{
    int idPessoa; 
    int rrn;
}Index;

int buscaBin(FILE* fp, int idPessoa);
FILE* openfile(char* filename, char* mode);
void printRegister( Registro registro);
int readRegister(FILE* fp, Registro *registro);
int func1(char* csv);
int func2(char *filename);
int* func3(char *file_bin, char * file_index, char *field, char *value);

int func5(char* file_bin, char* file_index, int n);


#endif