#include <stdio.h>
#include <string.h>

#define CANT_RESERVADAS 6

/* códigos de output*/
#define ERROR -1
#define SILENCIO 0
#define NORMAL 1
#define LOG 2
#define DEBUG 3

/* colores de output */
#define COLOR_ROJO     "\x1b[31m"
#define COLOR_VERDE   "\x1b[32m"
#define COLOR_AMARILLO  "\x1b[33m"
#define COLOR_AZUL    "\x1b[34m"
#define RESET_COLOR   "\x1b[0m"

/* ENCABEZADOS DE FUNCIONES */ 
int mostrarFatal(char* texto);
int mostrarNormal(char* texto);
int mostrarLog(char* texto);
int mostrarDebug(char* texto);
int validaParametros (int, char*[]);
FILE* abreArchivo (char*);
void leerArchivo(FILE*);
void buscaTokens(FILE*,char*[],int*);
int identificaTipoDeToken(char *token);
void creaPalabrasReservadas();

/* VARIABLES GLOBALES (sí, aguante!) */
int interactividad=DEBUG;	
char stringAuxiliar[100];
char *palabrasReservadas[CANT_RESERVADAS];
