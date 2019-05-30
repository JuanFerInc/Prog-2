/* 48191081 */
#include "../include/uso_cadena.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>


bool pertenece(int i, cadena_t cad) {
	return (es_localizador(siguiente_clave(i, inicio_cadena(cad), cad)));
}

nat longitud(cadena_t cad) {
	nat longitud = 0;
	if (!es_vacia_cadena(cad)) {
	localizador_t loc = inicio_cadena(cad);
		longitud++;
		while (!es_final_cadena(loc, cad)) {
			longitud++;
			loc = siguiente(loc, cad);
		}
	}
	return longitud;
}

bool son_iguales(cadena_t c1, cadena_t c2) {
	localizador_t loc1 = inicio_cadena(c1);
	localizador_t loc2 = inicio_cadena(c2);
	bool iguales = true;
	while (es_localizador(loc1) && es_localizador(loc2)) {
		if (numero_info (info_cadena(loc1, c1)) != numero_info( info_cadena(loc2, c2))) {
			iguales = false;
		}
		else if (strcmp(frase_info(info_cadena(loc1, c1)), frase_info(info_cadena(loc2, c2))) != 0){
			iguales = false;
		}
		loc1 = siguiente(loc1, c1);
		loc2 = siguiente(loc2, c2);
	}
	if ((es_localizador(loc1) && !es_localizador(loc2)) || (!es_localizador(loc1) && es_localizador(loc2))) {
		iguales = false;
	}

	return iguales;
}//end son_iguales (working)

cadena_t concatenar(cadena_t c1, cadena_t c2) {
	cadena_t res = crear_cadena();
	localizador_t loc1 = inicio_cadena(c1);
	localizador_t loc2 = inicio_cadena(c2);

	while(es_localizador(loc1)){
		info_t info = copia_info(info_cadena(loc1,c1));
		insertar_al_final(info,res);
		loc1 = siguiente(loc1,c1);
	}
	while (es_localizador(loc2)) {
		info_t info = copia_info(info_cadena(loc2,c2));
		insertar_al_final(info,res); 
		loc2 = siguiente(loc2, c2);
	}
	
	return res;
}

void ordenar(cadena_t &cad) {
	if (!es_vacia_cadena(cad)) {
		localizador_t loc1 = inicio_cadena(cad);
		localizador_t loc2;

		while (es_localizador(loc1)) {
			loc2 = siguiente(loc1, cad);
			while (es_localizador(loc2)) {
				if (numero_info(info_cadena(loc1, cad)) > numero_info(info_cadena(loc2, cad))) {
					//utilizamos intercambiar para ordenar la cadena sin cambiar de lugar los nodos
					intercambiar(loc1, loc2, cad);
				}
				loc2 = siguiente(loc2, cad);
			}
			loc1 = siguiente(loc1, cad);

			
		}

	}
}//end ordenar (working)



void cambiar_todos(int original, int nuevo, cadena_t &cad) {
	localizador_t loc = inicio_cadena(cad);
	if (!es_vacia_cadena(cad)) {
		while (es_localizador(loc)) {
			//si encontramos en numero original
			if (numero_info(info_cadena(loc,cad)) == original) {
				//necesitamos generar un nuevo arrelgo con la frase que contenia el nodo original
				//luego generamos un nuevo nodo, copiamos el dato numerico y enlasamos el nuevo arreglo
				//de esta forma no estamos utilisando el mismo arreglo de char, esto es un problema cuando
				//invocamos a liberar_info en el nodo con el dato numerico original
				char *nueva_frase = new char[strlen(frase_info(info_cadena(loc,cad)))+1];
				strcpy(nueva_frase,frase_info(info_cadena(loc,cad)));
				info_t dato = crear_info(nuevo,nueva_frase);
				info_t borrar = info_cadena(loc,cad);
				cambiar_en_cadena(dato,loc,cad);
				liberar_info(borrar);
			}
			
			loc = siguiente(loc,cad);
		}
	}
}


cadena_t subcadena(int menor, int mayor, cadena_t cad) {
	//inicio el localizador al comienso de cad
	localizador_t loc = inicio_cadena(cad);
	// creo una nueva cadena res
	cadena_t res = crear_cadena();
	while (es_localizador(loc) && (numero_info(info_cadena(loc, cad)) < menor)) {
		loc = siguiente(loc, cad);
	}
	while (es_localizador(loc) && (numero_info(info_cadena(loc, cad)) <= mayor)) {
		info_t copia = copia_info(info_cadena(loc,cad));
		insertar_al_final(copia, res);
		loc = siguiente(loc,cad);
	}

	return res;
}// end subcadena (working?)
