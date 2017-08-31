#include "funcionesAlex.h"
#include "regExpAlex.c"

/* DESARROLLO DE FUNCIONES */
int mostrarFatal(char* texto){
	printf (COLOR_ROJO"[FATAL]\t%s"RESET_COLOR,texto);
	return 1;
}

int mostrarNormal(char* texto){
	if (interactividad>=NORMAL){
		printf (RESET_COLOR"\t%s"RESET_COLOR,texto);
		if (interactividad==DEBUG){ fflush (stdin) ; scanf("%*c"); }
	}
	return 1;
}

int mostrarLog(char* texto){
	if (interactividad>=LOG)
		printf (COLOR_AMARILLO"[LOG]\t%s"RESET_COLOR,texto);
		if (interactividad==DEBUG){ fflush (stdin) ; scanf("%*c"); }
	return 1;
}

int mostrarDebug(char* texto){
	if (interactividad==DEBUG){
		printf (COLOR_VERDE"[DEBUG]\t%s"RESET_COLOR,texto);
		fflush (stdin) ; scanf("%*c");	
	}
	return 1;
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


 FILE* abreArchivoFuente (char* archivoFuente){
	FILE* pFuente= fopen (archivoFuente, "rt");
	if (! pFuente)
		return NULL;
	sprintf(stringAuxiliar,"Archivo [%s] abierto correctamente\n",archivoFuente);
	mostrarLog(stringAuxiliar);
	return pFuente;
}

 FILE* creaArchivoLex (char* archivoLex){
	FILE* pLex= fopen (archivoLex, "wt");
	if (! pLex)
		return NULL;
	sprintf(stringAuxiliar,"Archivo léxico [%s] abierto correctamente\n",archivoLex);
	mostrarLog(stringAuxiliar);
	return pLex;
}

void leerArchivo(FILE* archivoFuente){
	char caracter;
	while (  (caracter = fgetc(archivoFuente)) != EOF ) {
		printf ("-%c",caracter);
	}
}

void buscaTokens(FILE* archivoFuente,FILE* archivoLex,char* tiraDeTokens[],int* cantidadDeTokens){
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
			/*
			 *  verifico si el caracter es un + - * / = ( ) o algo así
			 * Debería emprolijar un poco esto, y el código que se repite, meterlo en una función.
			 */
			 
			if (caracter == '+' || caracter == '-' || caracter == '*' || caracter == '/' || caracter == '=' || caracter == '(' || caracter == ')'  ){
					if (!desplazamiento){ 
						/* Si está al principio el token, ES el token, así que lo inserto como un token nuevo */
						buffer[desplazamiento]=caracter;
						desplazamiento++;
						buffer[desplazamiento]='\0';
						sprintf(stringAuxiliar,"Nuevo Token(%d):  [%s]\n",*cantidadDeTokens,buffer);
						mostrarNormal(stringAuxiliar);
						tiraDeTokens[*cantidadDeTokens]=strdup(buffer);
						identificaTipoDeToken(archivoLex,tiraDeTokens[*cantidadDeTokens], *cantidadDeTokens);
						(*cantidadDeTokens)++;			
						desplazamiento=0;	
					}
					else{  //si venía en medio de un token Por ejemplo: "variable+variable"
						/* cierro el token actual */ 
						buffer[desplazamiento]='\0';
						sprintf(stringAuxiliar,"Nuevo Token(%d):  [%s]\n",*cantidadDeTokens,buffer);
						mostrarNormal(stringAuxiliar);
						tiraDeTokens[*cantidadDeTokens]=strdup(buffer);
						identificaTipoDeToken(archivoLex,tiraDeTokens[*cantidadDeTokens],*cantidadDeTokens);
						(*cantidadDeTokens)++;			
						desplazamiento=0;
						
						/* y lo inserto como nuevo token */
						buffer[desplazamiento]=caracter;
						desplazamiento++;
						buffer[desplazamiento]='\0';
						sprintf(stringAuxiliar,"Nuevo Token(%d):  [%s]\n",*cantidadDeTokens,buffer);
						mostrarNormal(stringAuxiliar);
						tiraDeTokens[*cantidadDeTokens]=strdup(buffer);
						identificaTipoDeToken(archivoLex,tiraDeTokens[*cantidadDeTokens],*cantidadDeTokens);
						(*cantidadDeTokens)++;			
						desplazamiento=0;			
					}
			}  /* Si estoy acá, continúo grabando el token */
			else {
				buffer[desplazamiento]=caracter;  //consumo un char en el buffer o lo ignoro
				desplazamiento++;
				sprintf(stringAuxiliar,"char [%c] consumido\n",caracter);
				mostrarLog(stringAuxiliar);
			}
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
				identificaTipoDeToken(archivoLex,tiraDeTokens[*cantidadDeTokens],*cantidadDeTokens);
				(*cantidadDeTokens)++;			
				desplazamiento=0;
			}
		}
	}
}

int identificaTipoDeToken (FILE* archivoLex, char *token, int numeroDeToken){
	sprintf(stringAuxiliar,"Identificando Token:  [%s]",token);
	mostrarDebug(stringAuxiliar);
	int tipoDeToken=TIPO_DESCONOCIDO;
	/* Hay algo extraño en esta parte, dadas las regex actuales, a veces matchea incorrectamente.
	 * Para solucionarlo, cambié el orden de las preguntas... así que ojo al cambiar el orden.
	 * Sí, ya sé... soy muy elegante.
	 */
	if (!tipoDeToken) (esPalabraReservada(token))?tipoDeToken=TIPO_PR :mostrarDebug("No es una Palabra Reservada");
	if (!tipoDeToken) (esOperador(token))?tipoDeToken=TIPO_OP:mostrarDebug("No es un operador");
	if (!tipoDeToken) (esTexto(token))?tipoDeToken=TIPO_TEXTO:mostrarDebug("No es texto");
	if (!tipoDeToken) (esNumeroEntero(token))?tipoDeToken=TIPO_ENT:mostrarDebug("No es un número entero");
	if (!tipoDeToken) (esNumeroFraccionario(token))?tipoDeToken=TIPO_REAL:mostrarDebug("No es un número real");
	if (!tipoDeToken) (esID(token))?tipoDeToken=TIPO_ID:mostrarDebug("No es una variable");

	if (!tipoDeToken){
		mostrarLog ("No se pudo identificar\n");
		return 0;
	}
	fprintf (archivoLex,"%d · %d · %s \n",numeroDeToken,tipoDeToken,token);
	sprintf (stringAuxiliar,"%d · %d · %s \n",numeroDeToken,tipoDeToken,token);
	mostrarDebug(stringAuxiliar);

	return 1 ;
}

void creaPalabrasReservadas(){
	palabrasReservadas[0]=strdup("BEGIN");
	palabrasReservadas[1]=strdup("END");
	palabrasReservadas[2]=strdup("FORT");
	palabrasReservadas[3]=strdup("TROF");
	palabrasReservadas[4]=strdup("WHILE");
	palabrasReservadas[5]=strdup("ELIHW");
}
	


