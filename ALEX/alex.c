#include "funcionesAlex.c"

/**********   MAIN    *************/

int main (int argc, char *argv[]){
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
}
