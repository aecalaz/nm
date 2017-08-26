#include "funcionesAlex.c"

/**********   MAIN    *************/
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
