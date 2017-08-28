#include "funcionesAlex.h"
#include "regExpAlex.c"


/* DESARROLLO DE FUNCIONES */
void mostrarFatal(char* texto){
	printf (COLOR_ROJO"[FATAL]\t%s"RESET_COLOR,texto);
}

void mostrarNormal(char* texto){
	if (interactividad>=NORMAL){
		printf (RESET_COLOR"\t%s"RESET_COLOR,texto);
		if (interactividad==DEBUG){ fflush (stdin) ; scanf("%*c"); }
	}
}

void mostrarLog(char* texto){
	if (interactividad>=LOG)
		printf (COLOR_AMARILLO"[LOG]\t%s"RESET_COLOR,texto);
		if (interactividad==DEBUG){ fflush (stdin) ; scanf("%*c"); }
}

void mostrarDebug(char* texto){
	if (interactividad==DEBUG){
		printf (COLOR_VERDE"[DEBUG]\t%s"RESET_COLOR,texto);
		fflush (stdin) ; scanf("%*c");	
	}
}

int validaParametros (int cantArgumentos, char* argumentos[]){
	if (cantArgumentos == 3) {
		if (!strcmp(argumentos[2],"--silencio")){ interactividad=SILENCIO; printf ("\n\nModo SILENCIO, no voy a molestar más.\n");}
		if (!strcmp(argumentos[2],"--normal")) 	{ interactividad=NORMAL; printf ("\n\nModo NORMAL, molestaré poco.\n");}
		if (!strcmp(argumentos[2],"--log")) 	{ interactividad=LOG; printf ("\n\nModo DEBUG, mostrando información de todo tipo.\n");}
		if (!strcmp(argumentos[2],"--debug"))	{ interactividad=DEBUG ; printf ("\n\nModo DEBUG, presione enter para avanzar un renglón\n");}
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
		sprintf(stringAuxiliar,"Error al abrir el archivo [%s]\n",archivoFuente);
		mostrarNormal(stringAuxiliar);
		return NULL;
		}
	sprintf(stringAuxiliar,"Archivo [%s] abierto correctamente\n",archivoFuente);
	mostrarLog(stringAuxiliar);
	return pFuente;
}

void leerArchivo(FILE* archivoFuente){
	char caracter;
	while (  (caracter = fgetc(archivoFuente)) != EOF ) {
		printf ("-%c",caracter);
	}
}

void buscaTokens(FILE* archivoFuente,char* tiraDeTokens[],int* cantidadDeTokens){
	creaPalabrasReservadas();
	mostrarNormal("Buscando Tokens\n");
	char caracter;
	char buffer[100];  //preferiría no tener variables de más de 100 caracteres...
	int contadorDeLetras=0;
	int desplazamiento=0;
	
	while ((caracter = fgetc(archivoFuente)) != EOF ) {
		contadorDeLetras++;
		if (caracter != '\n' )	{sprintf(stringAuxiliar,"char [%c] -- lectura número %d\n",caracter,contadorDeLetras);}
		else 					{sprintf(stringAuxiliar,"char [NEWLINE] -- lectura número %d\n",contadorDeLetras);}
		mostrarLog(stringAuxiliar);
		
		if (caracter != ' ' && caracter != '\n' && caracter != '\t'){
			buffer[desplazamiento]=caracter;  //consumo un char en el buffer o lo ignoro
			desplazamiento++;
			sprintf(stringAuxiliar,"char [%c] consumido\n",caracter);
			mostrarLog(stringAuxiliar);
		}
		else{
			if (desplazamiento) {
				/* 
				 * Si llegué hasta acá, encontré un nuevo token. Lo marco como '\0' y me fijo
				 * qué tipo de token es utilizando las funciones de regExp
				 */
				 
				buffer[desplazamiento]='\0';
				sprintf(stringAuxiliar,"Nuevo Token(%d):  [%s]\n",*cantidadDeTokens,buffer);
				mostrarNormal(stringAuxiliar);
				tiraDeTokens[*cantidadDeTokens]=strdup(buffer);
				//identificaTipoDeToken(tiraDeTokens[*cantidadDeTokens],palabrasReservadas);
				(*cantidadDeTokens)++;			
				desplazamiento=0;
			}
		}
	}
}

void identificaTipoDeToken (char* fruta[]){
	mostrarDebug("Identificando Token\n");
	}

void creaPalabrasReservadas(){
	palabrasReservadas[0]=strdup("BEGIN");
	palabrasReservadas[1]=strdup("IF");
	palabrasReservadas[2]=strdup("FORT");
	palabrasReservadas[3]=strdup("SARASA");
	palabrasReservadas[4]=strdup("VIVA");
	palabrasReservadas[5]=strdup("PERÓN");
}
	
	
	
/*
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
*/


