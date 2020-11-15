#ifndef AUX_FUNC_H   // guardas de cabeçalho, impedem inclusões cíclicas
#define AUX_FUNC_H

Lista* criaLista();
void liberaLista(Lista* list);
void escreveLista(FILE* fp, Lista *list);
int insereListaOrdenado(Lista* list, Index ind);
void imprimeLista(Lista *list);

void remove_quotes(char * line);
int buscaBin(FILE* fp, int idPessoa);
FILE* openfile(char* filename, char* mode);
void binarioNaTela1(char *nomeArquivoBinario, char *nomeArquivoIndice);
void scan_quote_string(char *str);
void writeFilled(FILE* fp, char* str, int SIZE, int lixo);

void deleteFrom(Lista* list, Index ind);
Lista* read2Lista(FILE* fp);

#endif