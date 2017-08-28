#include "regExpAlex.h"

void ejemploRegExp(){
	/*
	 * Función trivial para entender cómo usar las expresiones regulares
	 * Para molestar, y entender un poquito más cómo funciona, habilito el output de --debug
	 *
	 */
	 
	int interactividadOriginal=interactividad;
	interactividad=DEBUG;
	
	mostrarDebug("Estamos en modo DEBUG, presione enter repetidas veces para avanzar");
	
	char *loteDePruebas[5]={	\
					strdup("123"),\
					strdup("123.45"),	\
					strdup("123."),	\
					strdup("hola"), \
					strdup("hola_1")	\
					};
	
	
	for (int i = 0 ; i < 5 ; i++) {
		(esNumeroEntero(loteDePruebas[i]))?sprintf(stringAuxiliar,"%s es entero\n",loteDePruebas[i]):sprintf(stringAuxiliar,"%s no es entero\n",loteDePruebas[i]);
		mostrarLog(stringAuxiliar);
		(esNumeroFraccionario(loteDePruebas[i]))?sprintf(stringAuxiliar,"%s es fraccionario\n",loteDePruebas[i]):sprintf(stringAuxiliar,"%s no es fraccionario\n",loteDePruebas[i]);
		mostrarLog(stringAuxiliar);
		(esTexto(loteDePruebas[i]))?sprintf(stringAuxiliar,"%s es palabra\n",loteDePruebas[i]):sprintf(stringAuxiliar,"%s no es palabra\n",loteDePruebas[i]);
		mostrarLog(stringAuxiliar);
		(esID(loteDePruebas[i]))?sprintf(stringAuxiliar,"%s es un ID \n",loteDePruebas[i]):sprintf(stringAuxiliar,"%s no es un ID\n",loteDePruebas[i]);
		mostrarLog(stringAuxiliar);
	}

 	
 	 //suficientes ejemplos	
	interactividad=interactividadOriginal;
	}


int buscaRegExp(char* cadenaParaAnalizar, char *regExp) {
	
/*
 * Función genérica que recibe dos cadenas, una a analizar, y la otra es la regexp que va a matchear.
 * 
 * Devuelve la cantidad de coincidencias.
 * Por ejemplo:
 * buscaRegExp ("hola","[ol]"); 
 * va a devolver 1, porque sólo matchea la "o".
 * 
 * Pero
 * buscaRegExp ("hola","[ol]+");
 * va a devolver 2, porque matchea "ol".
 * 
 * Esto sirve para comparar con strlen de la palabra a analizar.
 * 
 * Sugiero no utilizar esta función directamente, sino enmascarada en otra función que asegure que 
 * la regexp usada tenga sentido y no haga que explote todo.
 * 
 */

/*  Machete de expresiones regulares
 * 
 * -?[0-9]+(\\.[0-9]+)?     //verificar qué demonios hace estar regex
 * 
 * One of         [AB]    one of A or B
 * Not one of     [^AB]   anything but A and B (new line may or may not be included, implementation or option specific)
 * Zero or more   A*      any number of A's including 0
 * Group          (A)     Used for grouping things like OR
 * Any character  .       Any single character (not including new line)
 * 
 */
	
// regexec() returns 0 on match, otherwise REG_NOMATCH
// ¿mantengo el estándar de devolver 0 al matchear, o hago lo humanamente entendible?
	
	int rv;
	regex_t expresion;
	rv = regcomp(&expresion, regExp, REG_EXTENDED);  //compila la regexp que recibe por parámetro
		
	regmatch_t matches[MAX_MATCHES]; //A list of the matches in the string (a list of 1)
	if (regexec(&expresion, cadenaParaAnalizar, MAX_MATCHES, matches, 0) == 0) 
		sprintf(stringAuxiliar,"\"%s\" la expresión regular matcheó entre los caracteres %d y %d con la expresión \"%s\"", cadenaParaAnalizar, matches[0].rm_so, matches[0].rm_eo, regExp) ;
	else 
		sprintf(stringAuxiliar,"\"%s\" no hubo match en \"%s\" :(", cadenaParaAnalizar,regExp);	
	mostrarDebug(stringAuxiliar);

	return (matches[0].rm_eo-matches[0].rm_so); //cantidad de coincidencias

}


/*
 * Las funciones que vienen a continuación (que arrancan con "es" 
 * Enmascaran la función buscaRegExp de manera ordenada, para averiguar si el token enviado
 * es un número, una cadena de texto, algo mezclado, o lo que sea.
 * Devuelven 0 si el token enviado no coincide en un 100% con el tipo buscado, o un 1 en caso de coincidir.
 */

 
int esNumeroEntero(char* token){
	int coincidencias=buscaRegExp(token,"[0-9]+");
	if (coincidencias && (coincidencias == strlen (token) ))
		return 1;  // es un caso válido
	return 0;      // no es una caso válido	
}

int esNumeroFraccionario(char* token){
	int coincidencias=buscaRegExp(token,"[0-9]+\\.[0-9]+");
	if (coincidencias && (coincidencias == strlen (token) ))
		return 1;  // es un caso válido
	return 0;      // no es una caso válido	
}


int esTexto(char* token){
	//sólo texto sin espacios, mayúsculas o minúsculas
	int coincidencias=buscaRegExp(token,"[a-zA-Z]+");
	if (coincidencias && (coincidencias == strlen (token) ))
		return 1;  // es un caso válido
	return 0;      // no es una caso válido	
}

int esID(char* token){
	// Arranca con una letra y sigue con letras o números
	int coincidencias=buscaRegExp(token,"[a-zA-Z]([a-zA-Z]|[0-9]|_|-)*");
	if (coincidencias && (coincidencias == strlen (token) ))
		return 1;  // es un caso válido
	return 0;      // no es una caso válido	
}

int esPalabraReservada(char* token){
	/*
	 * Esto es mentira, no uso RegExp, sino que comparo directamente con strcmp. 
	 * Pero la dejo acá por una cuestión de homogeneidad.
	 */
	 if (strlen (token)) { // verifico que no sea una palabra vacía.
		 for (int j=0; j<CANT_RESERVADAS ; j++){
			if (! strcmp (token,palabrasReservadas[j])){
					return 1;  // es una coincidencia al 100%
			}	
		}
	}
	 
	return 0;	
}
