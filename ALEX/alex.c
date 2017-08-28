#include "funcionesAlex.c"



/**********   MAIN    *************/
int main (int argc, char *argv[]){
   printf("Interactividad: %d",interactividad);
   //ejemploRegExp();
	

	
	//return 0;
	
	
	FILE* archivoFuente,*principioArchivoFuente;


	char *tiraDeTokens[100];  // inicialmente, quiero ver que funcione con 100 tokens.
	int cantidadDeTokens=0;	
	if (!validaParametros(argc,argv)){
		mostrarFatal ("Error de parámetros, saliendo.\n");
		return 0;
	}

// Si estoy acá, los parámetros fueron correctos, podemos intentar abrir el archivo fuente.
	archivoFuente = abreArchivo (argv[1]);
	if (!archivoFuente) {
		mostrarFatal ("Error al abrir el archivo fuente, saliendo.\n");
		mostrarFatal (argv[1]);
		return 0;
	}
	
/* Si estoy acá, es porque el archivo existe y se pudo abrir correctamente. Hay que leerlo y empezar a jugar. */
	buscaTokens(archivoFuente,tiraDeTokens,&cantidadDeTokens);  //cantidad por referencia
	fclose (archivoFuente);
	
/* Ya terminé de leer la tira de tokens, ahora me fijo si hay palabras reservadas */	
// esta función ya no se usa, ahora jugamos con regExp. ;)
//	buscaPalabrasReservadas(palabrasReservadas,tiraDeTokens,cantidadDeTokens);  // cantidad por copia
	
	
/* FIN DEL PROGRAMA*/
}
