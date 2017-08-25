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
void mostrarFatal(char* texto);
void mostrarNormal(char* texto);
void mostrarLog(char* texto);
void mostrarDebug(char* texto);
int validaParametros (int, char*[]);
FILE* abreArchivo (char*);
void leerArchivo(FILE*);
void buscaTokens(FILE*,char*[],char*[],int*);
void buscaPalabrasReservadas(char*[],char*[],int);

/* VARIABLES GLOBALES (sí, aguante!) */
int interactividad=ERROR;	
char stringAuxiliar[100];



/* MAIN */
int main (int argc, char *argv[]){
	FILE* archivoFuente,*principioArchivoFuente;

// declarar acá todas las palabras reservadas del lenguaje, y acordarse de aumentar el tamaño de la constante CANT_RESERVADAS... :P	
	char *palabrasReservadas[CANT_RESERVADAS]={	\
					strdup("BEGIN"),\
					strdup("NIGEB"),	\
					strdup("IF"),	\
					strdup("FI"), \
					strdup("FORT"),	\
					strdup("TROF")
					};
	char *tiraDeTokens[100];  // inicialmente, quiero ver que funcione con 100 tokens.
	int cantidadDeTokens=0;	
	if (!validaParametros(argc,argv)){
		mostrarFatal ("Error de parámetros, saliendo.");
		return 0;
	}

// Si estoy acá, los parámetros fueron correctos, podemos intentar abrir el archivo fuente.
	archivoFuente = abreArchivo (argv[1]);
	if (!archivoFuente) {
		mostrarFatal ("Error al abrir el archivo fuente, saliendo.");
		mostrarFatal (argv[1]);
		return 0;
	}
	
/* Si estoy acá, es porque el archivo existe y se pudo abrir correctamente. Hay que leerlo y empezar a jugar. */
	buscaTokens(archivoFuente,palabrasReservadas,tiraDeTokens,&cantidadDeTokens);  //cantidad por referencia
	fclose (archivoFuente);
	
/* Ya terminé de leer la tira de tokens, ahora me fijo si hay palabras reservadas */	
	buscaPalabrasReservadas(palabrasReservadas,tiraDeTokens,cantidadDeTokens);  // cantidad por copia
	
	
/* FIN DEL PROGRAMA*/
}


/* DESARROLLO DE FUNCIONES */
void mostrarFatal(char* texto){
	printf (COLOR_ROJO"[FATAL]\t%s\n"RESET_COLOR,texto);
}

void mostrarNormal(char* texto){
	if (interactividad>=NORMAL){
		printf (RESET_COLOR"\t%s\n"RESET_COLOR,texto);
		if (interactividad==DEBUG){ fflush (stdin) ; scanf("%*c"); }
	}
}

void mostrarLog(char* texto){
	if (interactividad>=LOG)
		printf (COLOR_AMARILLO"[LOG]\t%s\n"RESET_COLOR,texto);
		if (interactividad==DEBUG){ fflush (stdin) ; scanf("%*c"); }
}

void mostrarDebug(char* texto){
	if (interactividad==DEBUG)
		printf ("COLOR_VERDE[DEBUG]\t%s\n"RESET_COLOR,texto);
		fflush (stdin) ; scanf("%*c");	
}

int validaParametros (int cantArgumentos, char* argumentos[]){
	if (cantArgumentos == 3) {
		if (!strcmp(argumentos[2],"--silencio")) 	 interactividad=SILENCIO;
		if (!strcmp(argumentos[2],"--normal")) 	 interactividad=NORMAL;
		if (!strcmp(argumentos[2],"--log")) 		 interactividad=LOG;
		if (!strcmp(argumentos[2],"--debug")){ 	 interactividad=DEBUG ; printf ("\n\nModo DEBUG, presione enter para avanzar un renglón\n");}
		if (interactividad == ERROR){
			printf ("[Valor ingresado: %s] Error de argumentos: arg2 debe ser --silencio, --normal, --log o --debug; dependiendo de las ganas de leer.\n\n",argumentos[2]);
			return 0; 
		}
	}
	else {
		printf (" ***** Error de argumentos ***** \n\tUso correcto: ./alex miProgramaFuente.nm --normal\n");
		printf (" \tAdmite --silencio --normal --log o --debug, dependiendo de las ganas de recibir mensajes que tengas.\n");
		return 0; 
	}
	return 1;
}


 FILE* abreArchivo (char* archivoFuente){
	FILE* pFuente= fopen (archivoFuente, "rt");
	if (! pFuente){
		sprintf(stringAuxiliar,"Error al abrir el archivo [%s]",archivoFuente);
		mostrarNormal(stringAuxiliar);
		return NULL;
		}
	sprintf(stringAuxiliar,"Archivo [%s] abierto correctamente",archivoFuente);
	mostrarLog(stringAuxiliar);
	return pFuente;
}

void leerArchivo(FILE* archivoFuente){
	char caracter;
	while (  (caracter = fgetc(archivoFuente)) != EOF ) {
		printf ("-%c",caracter);
	}
}

void buscaTokens(FILE* archivoFuente,char* reservadas[],char* tiraDeTokens[],int* cantidadDeTokens){
	mostrarNormal("Buscando Tokens");
	char caracter;
	char buffer[100];  //preferiría no tener variables de más de 100 caracteres...
	int contadorDeLetras=0;
	int desplazamiento=0;
	
	while ((caracter = fgetc(archivoFuente)) != EOF ) {
		contadorDeLetras++;
		if (caracter != '\n' )	{sprintf(stringAuxiliar,"char [%c] -- lectura número %d",caracter,contadorDeLetras);}
		else 					{sprintf(stringAuxiliar,"char [NEWLINE] -- lectura número %d",contadorDeLetras);}
		mostrarLog(stringAuxiliar);
		
		if (caracter != ' ' && caracter != '\n' && caracter != '\t'){
			buffer[desplazamiento]=caracter;  //consumo un char en el buffer o lo ignoro
			desplazamiento++;
			sprintf(stringAuxiliar,"char [%c] consumido",caracter);
			mostrarLog(stringAuxiliar);
		}
		else{
			if (desplazamiento) {
				buffer[desplazamiento]='\0';
				sprintf(stringAuxiliar,"Nuevo Token(%d):  [%s] ",*cantidadDeTokens,buffer);
				mostrarNormal(stringAuxiliar);
				tiraDeTokens[*cantidadDeTokens]=strdup(buffer);
				(*cantidadDeTokens)++;			
				desplazamiento=0;
			}
		}
	}
}

void buscaPalabrasReservadas(char*palabrasReservadas[],char* tiraDeTokens[],int cantidadDeTokens){
		for (int i=0; i<cantidadDeTokens ; i++){
			for (int j=0; j<CANT_RESERVADAS ; j++){
				if (! strcmp (tiraDeTokens[i],palabrasReservadas[j])){
					sprintf(stringAuxiliar,"Palabra Reservada [%s] en el token [%d] ",tiraDeTokens[i],i);
					mostrarNormal(stringAuxiliar);
				}
			}
		}
}

