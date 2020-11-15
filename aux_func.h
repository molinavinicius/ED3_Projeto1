#ifndef AUX_FUNC_H   // guardas de cabeçalho, impedem inclusões cíclicas
#define AUX_FUNC_H

void remove_quotes(char * line);
int buscaBin(FILE* fp, int idPessoa);
FILE* openfile(char* filename, char* mode);
void binarioNaTela1(char *nomeArquivoBinario, char *nomeArquivoIndice);
void scan_quote_string(char *str);

#endif