#include <regex.h>
#define MAX_MATCHES 10  

int buscaRegExp(char* cadenaParaAnalizar, char *regExp);
int esNumeroEntero(char* token);
int esNumeroFraccionario(char* token);
int esTexto(char* token);
int esID(char* token);
int esPalabraReservada(char* token);  // no una regexp, pero la dejo igual
