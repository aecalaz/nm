#include "funcionesAlex.c"

/**********   MAIN    *************/

int main (int argc, char *argv[]){
	FILE* archivoFuente,*archivoLex;
	char *tiraDeTokens[100];  // inicialmente, quiero ver que funcione con 100 tokens.
	int cantidadDeTokens=0;	
	char nombreArchivoLex[100];

	if (!validaParametros(argc,argv)){
		mostrarFatal ("Error de parámetros, saliendo.\n");
		return 0;
	}

// Si estoy acá, los parámetros fueron correctos, podemos intentar abrir el archivo fuente.
	archivoFuente = abreArchivoLectura (argv[1]);
	if (!archivoFuente) {
		sprintf(stringAuxiliar,"Error al abrir el archivo fuente [%s], saliendo.\n\n",archivoFuente);
		mostrarFatal (stringAuxiliar);
		return 0;
	}

	sprintf(nombreArchivoLex,"%s.lex",argv[1]);
	archivoLex = creaArchivoNuevo (nombreArchivoLex);
	if (!archivoLex) {
		sprintf(stringAuxiliar,"Error al abrir el archivo léxico [%s], saliendo.\n\n",archivoLex);
		mostrarFatal (stringAuxiliar);
		return 0;
	}
		
/* Si estoy acá, es porque el archivo existe y se pudo abrir correctamente. Hay que leerlo y empezar a jugar. */
	buscaTokens(archivoFuente,archivoLex,tiraDeTokens,&cantidadDeTokens);  //cantidad por referencia
	fclose(archivoLex);
	fclose (archivoFuente);
	
/* Abro nuevamente el archivo léxico, para agrupar términos y esas cosas */	
	archivoLex = abreArchivoLectura (nombreArchivoLex);
	if (!archivoLex) {
		sprintf(stringAuxiliar,"Error al abrir el archivo léxico [%s] para agrupar cosas, saliendo.\n\n",archivoLex);
		mostrarFatal (stringAuxiliar);
		return 0;
	}
	
	agrupaTokens(archivoLex,&cantidadDeTokens);
}
