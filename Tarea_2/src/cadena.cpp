#include "../include/cadena.h"
#include "../include/info.h"

#include <stddef.h>
#include <stdio.h>
#include <assert.h>



//lista doblemente encandenada que contiene elementos del tipo info_t
struct nodo {
	info_t dato;
	nodo *anterior;
	nodo *siguiente;
};

// nodo que contiene el comienzo y final de una cadena de nodos
struct rep_cadena {
	nodo *inicio;
	nodo *final;
};

typedef struct rep_cadena *cadena_t;

cadena_t crear_cadena() {
	// allocates space for a node of type rep_cadena
	cadena_t nueva_cadena = new rep_cadena;
	//points both inicio and final to NULL
	nueva_cadena->inicio = NULL;
	nueva_cadena->final = NULL;

	//return the pointer of type rep_cadean pointing to NULL
	return nueva_cadena;
}//end crear_cadena



void insertar_al_final(info_t i, cadena_t &cad) {
	nodo *nuevo = new nodo;
	nuevo->dato = i;
	nuevo->siguiente = NULL;
	nuevo->anterior = cad->final;
	if (cad->final == NULL) {
		assert(cad->inicio == NULL);
		cad->inicio = nuevo;
	}//end if
	else {
		assert(cad->inicio != NULL);
		cad->final->siguiente = nuevo;
	}//end else
	cad->final = nuevo;
}//end insertar_al_final (working)

void insertar_antes(info_t i, localizador_t loc, cadena_t &cad) {
	assert(localizador_en_cadena(loc,cad));
	localizador_t nuevo = new nodo;
	nuevo->dato = i;
	nuevo->siguiente = loc;

	if (loc->anterior == NULL) {
		loc->anterior = nuevo;
		nuevo->anterior = NULL;
		cad->inicio = nuevo;
	}
	else {
		nuevo->anterior = loc->anterior;
		loc->anterior->siguiente = nuevo;
		loc->anterior = nuevo;
	}
}// end insertar_antes

void insertar_segmento_despues(cadena_t &sgm, localizador_t loc, cadena_t &cad) {
	assert((es_vacia_cadena(cad)) || localizador_en_cadena(loc, cad));
	if (es_vacia_cadena(cad)) {
		cad->inicio = sgm->inicio;
		cad->final = sgm->final;
	}else {
		if (!es_vacia_cadena(sgm)) {
			sgm->inicio->anterior = loc;
			sgm->final->siguiente = loc->siguiente;
			if (es_final_cadena(loc, cad)) {
				cad->final = sgm->final;
			}
			else {
				loc->siguiente->anterior = sgm->final;
			}
			loc->siguiente = sgm->inicio;
		}

	}//end else
	sgm->inicio = sgm->final = NULL;
}//end insertar_segmento_despues


cadena_t segmento_cadena(localizador_t desde, localizador_t hasta, cadena_t cad) {
	assert(es_vacia_cadena(cad) || precede_en_cadena(desde, hasta, cad));
	cadena_t res = crear_cadena();
	if (!es_vacia_cadena(cad)) {
		localizador_t loc = desde;
		while (loc != siguiente(hasta, cad)) {
			//asignamos una copia
			info_t info = copia_info((loc->dato));
			insertar_al_final(info, res);
			loc = siguiente(loc, cad);
		}
	}
	return res;
}//end segmento_cadena

cadena_t separar_segmento(localizador_t desde, localizador_t hasta, cadena_t &cad) {
	assert(es_vacia_cadena(cad) || precede_en_cadena(desde, hasta, cad));
	cadena_t sol = crear_cadena();
	if (!es_vacia_cadena(cad)) {
		sol->inicio = desde;
		sol->final = hasta;
		if ((cad->inicio == desde) && (cad->final == hasta)) {
			cad->inicio = cad->final = NULL;
		}
		else if (desde->anterior == NULL) {
			cad->inicio = desde->siguiente;
		}
		else if (hasta->siguiente == NULL) {
			cad->final = desde->anterior;
		}
		else {
			desde->anterior->siguiente = hasta->siguiente;
			hasta->siguiente->anterior = desde->anterior;

		}
		desde->anterior = NULL;
		hasta->siguiente = NULL;
	}
	return sol;
}//end separar_segmento



cadena_t mezcla(cadena_t c1, cadena_t c2) {
	
	assert(esta_ordenada(c1) && esta_ordenada(c2));

	cadena_t res = crear_cadena();
	//posicionamos un localizador en el comienso de cada cadena
	localizador_t p1 = c1->inicio;
	localizador_t p2 = c2->inicio;
	
	info_t info;
	// asumimos que las dos cadenan tienen elementos 
	// y comensamos a copiar de forma ordenada
	while (es_localizador(p1) && es_localizador(p2)) {
		if (numero_info(info_cadena(p1,c1)) <= numero_info(info_cadena(p2,c2))) {
			
			info = copia_info(info_cadena(p1,c1));
			insertar_al_final(info, res);
			p1 = siguiente(p1, c1);
			
		}
		else {
			info = copia_info(info_cadena(p2,c2));
			insertar_al_final(info, res);
			p2 = siguiente(p2, c2);
		}
	}//end while
	// al salir del while tenemos 2 casos
	// llegamos al final de la segunda cadena y quedan elementos en la primera
	if (es_localizador(p1)) {
		while (es_localizador(p1)) {
			info = copia_info(info_cadena(p1,c1));
			insertar_al_final(info, res);
			p1 = siguiente(p1, c1);
		}
	}
	//llegamos al final de la primera cadena y quedan elementos por copiar de la segunda
	else if (es_localizador(p2)) {
		while(es_localizador(p2)){
			info = copia_info(info_cadena(p2,c2));
			insertar_al_final(info, res);
			p2 = siguiente(p2, c2);
		}	
	}


	return res;
}//end mezcla (working)


void remover_de_cadena(localizador_t &loc, cadena_t &cad) {
	assert(localizador_en_cadena(loc, cad));
	if (es_inicio_cadena(loc,cad)) {
		cad->inicio = cad->inicio->siguiente;
		if(cad->inicio != NULL){
			cad->inicio->anterior = NULL;
		}else{
			cad->final = NULL;
		}
	}
	else if (es_final_cadena(loc,cad)) {
		cad->final = cad->final->anterior;
		cad->final->siguiente = NULL;
	}
	else {
		loc->anterior->siguiente = loc->siguiente;
		loc->siguiente->anterior = loc->anterior;
	}
	liberar_info(loc->dato);
	delete(loc);
	loc = NULL;

}//end remove_de_cadena (working)

void liberar_cadena(cadena_t &cad) {
	nodo *a_borrar;
	while (cad->inicio != NULL) {
		a_borrar = cad->inicio;
		cad->inicio = cad->inicio->siguiente;
		liberar_info(a_borrar->dato);
		delete(a_borrar);
	}
	delete cad;
}//end libera_cadena


bool es_localizador(localizador_t loc) {
	return loc != NULL;
}//end es__localizador (working)


bool es_vacia_cadena(cadena_t cad) {
	return (cad->inicio == NULL) && (cad->final == NULL);
}//end es_vacia_cadena (working)


bool esta_ordenada(cadena_t cad) {
	bool res = true;
	if (!es_vacia_cadena(cad)) {
		localizador_t loc = inicio_cadena(cad);
		while (res && es_localizador(siguiente(loc, cad))) {
			localizador_t sig_loc = siguiente(loc, cad);
			if (numero_info(info_cadena(loc, cad)) > numero_info(info_cadena(sig_loc, cad))) {
				res = false;
			}
			else loc = siguiente(loc, cad);
		}
	}
	return res;
}//end esta_ordenada (working)


bool es_final_cadena(localizador_t loc, cadena_t cad) {
	return (!es_vacia_cadena(cad) && (loc == cad->final));
}//end es_final_cadena


bool es_inicio_cadena(localizador_t loc, cadena_t cad) {
	return  (!es_vacia_cadena(cad) && (loc == cad->inicio));
}//end es_inicio_cadena


bool localizador_en_cadena(localizador_t loc, cadena_t cad) {
	bool esta = false;
	if (!es_vacia_cadena(cad)) {
		nodo *busco = cad->inicio;
		while ((esta == false) && (busco != NULL)) {
			if (busco == loc) {
				esta = true;
			}
			busco = busco->siguiente;
		}
	}
	return esta;
}//end localizador_en_cadena (working)


bool precede_en_cadena(localizador_t loc1, localizador_t loc2, cadena_t cad) {
	localizador_t cursor = loc1;
	while (es_localizador(cursor) && (cursor != loc2)) {
		cursor = siguiente(cursor, cad);
	}
	return ((cursor == loc2) && (localizador_en_cadena(loc1, cad)));
}//end precede_en_cadena (working)


localizador_t inicio_cadena(cadena_t cad) {
	//localizador_t inicio = NULL;
	//if (!es_vacia_cadena(cad)) {
	//	inicio = cad->inicio;
	//}
	localizador_t inicio = cad->inicio;
	return inicio;
}//end inicio_cadena (working)

localizador_t final_cadena(cadena_t cad) {
	localizador_t final = NULL;
	if (!es_vacia_cadena(cad)) {
		final = cad->final;
	}
	return final;
}//end final_cadena (working)


localizador_t kesimo(nat k, cadena_t cad) {
	localizador_t loc;
	// caso en el que k sea 0
	if (k == 0) {
		loc = NULL;
	}
	// cubre el resto de los casos, en el que kesimo esta en la cadena
	// y en el que k es mayor al largo de la cadena
	else {
		loc = cad->inicio;
		k--;
		while (loc != NULL && k != 0) {
			loc = siguiente(loc,cad);
			k--;
		}
	}
	return loc;
}//end kesimo (working)


localizador_t siguiente(localizador_t loc, cadena_t cad) {
	assert(localizador_en_cadena(loc, cad));
	if (es_final_cadena(loc, cad)) {
		return NULL;
	}
	else {
		return loc->siguiente;
	}
}//end siguiente


localizador_t anterior(localizador_t loc, cadena_t cad) {
	assert(localizador_en_cadena(loc, cad));
	if (es_inicio_cadena(loc, cad)) {
		return NULL;
	}
	else {
		return loc->anterior;
	}
}//end anterior


localizador_t menor_en_cadena(localizador_t loc, cadena_t cad) {
	assert(localizador_en_cadena(loc, cad));
	localizador_t res = loc;
	while (es_localizador(siguiente(loc, cad))) {
		loc = siguiente(loc, cad);
		if (numero_info(info_cadena(loc, cad)) < numero_info(info_cadena(res, cad))) {
			res = loc;
		}
	}
	return res;
}//end menor_en_cadena

localizador_t siguiente_clave(int clave, localizador_t loc, cadena_t cad) {
	assert(es_vacia_cadena(cad) || localizador_en_cadena(loc, cad));
	localizador_t res = loc;
	if (es_vacia_cadena(cad)) {
		res = NULL;
	}
	else {
		while (es_localizador(res) && numero_info(info_cadena(res, cad)) != clave) {
			res = siguiente(res, cad);
		}
	}
	return res;
}//end siguiente_clave


localizador_t anterior_clave(int clave, localizador_t loc, cadena_t cad) {
	assert(es_vacia_cadena(cad) || localizador_en_cadena(loc, cad));
	localizador_t res;
	while (es_localizador(loc) && (clave != numero_info(info_cadena(loc, cad)))) {
			loc = anterior(loc, cad);
		}
		res = loc;
		return res;
	}//end anterior_cave


info_t info_cadena(localizador_t loc, cadena_t cad) {
	assert(localizador_en_cadena(loc, cad));
	if(es_localizador(loc)){
		return loc->dato;
	}
	else return NULL;
}//end info_cadena (working)


void cambiar_en_cadena(info_t i, localizador_t loc, cadena_t &cad) {
	assert(localizador_en_cadena(loc, cad));
	loc->dato = i;
}//end cambiar_en_cadena

void intercambiar(localizador_t loc1, localizador_t loc2, cadena_t &cad) {
	assert(localizador_en_cadena(loc1, cad) && localizador_en_cadena(loc2, cad));
	info_t aux = loc1->dato;
	loc1->dato = loc2->dato;
	loc2->dato = aux;
}//end intercambiar


void imprimir_cadena(cadena_t cad) {
	localizador_t loc = inicio_cadena(cad);
	while (es_localizador(loc) ){
		printf("(%d,%s)", numero_info(loc->dato),frase_info(loc->dato));
		loc = siguiente(loc, cad);
	}
	printf("\n");
}//end imprimir_cadena (working)
