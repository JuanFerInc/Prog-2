/* 48191081 */
#include "stdio.h"
#include "../include/tabla.h"

struct rep_nodo_tabla{
	cadena_t cad;
};

struct rep_tabla {
	rep_nodo_tabla *arr;
	nat tam_tabla;
	nat cant_elem;
};

typedef rep_tabla *tabla_t;

tabla_t crear_tabla(nat tamanio){
	tabla_t t = new rep_tabla;
	t->arr = new rep_nodo_tabla[tamanio];
	t->tam_tabla = tamanio;
	t->cant_elem = 0;

	for(nat i = 0; i < tamanio; i++){
		t->arr[i].cad =crear_cadena();
	}

	return t;
}

void asociar_en_tabla(int clave, char *valor, tabla_t &t){
	int pos = clave % t->tam_tabla;
	 
	info_t info = crear_info(clave,valor);
	localizador_t loc = inicio_cadena(t->arr[pos].cad);
	
	loc = siguiente_clave(clave,loc,t->arr[pos].cad);

	if(es_localizador(loc)){
		remover_de_cadena(loc,t->arr[pos].cad);
		t->cant_elem--;
	}

	insertar_al_final(info,t->arr[pos].cad);
	t->cant_elem++;
}

void eliminar_de_tabla(int clave, tabla_t &t){
	int pos = clave % t->tam_tabla;
	
	localizador_t loc = inicio_cadena(t->arr[pos].cad);
	
	loc = siguiente_clave(clave,loc,t->arr[pos].cad);

	if(es_localizador(loc)){
		remover_de_cadena(loc,t->arr[pos].cad);
		t->cant_elem--;
	}


}

void liberar_tabla(tabla_t &t){
	for(nat i = 0; i < t->tam_tabla; i++){
		liberar_cadena(t->arr[i].cad);
	}
	delete[] t->arr;
	delete t;
}

bool existe_asociacion(int clave, tabla_t t){
	int pos = clave % t->tam_tabla;
	localizador_t loc = inicio_cadena(t->arr[pos].cad);
	loc = siguiente_clave(clave,loc,t->arr[pos].cad);
	if(es_localizador(loc)){
		return true;
	}else return false;
}

char *valor_en_tabla(int clave, tabla_t t){
	int pos = clave % t->tam_tabla;
	localizador_t loc = inicio_cadena(t->arr[pos].cad);
	loc = siguiente_clave(clave,loc,t->arr[pos].cad);
	return frase_info(info_cadena(loc,t->arr[pos].cad));
}

bool esta_llena_tabla(tabla_t t){
	if(t->cant_elem >= t->tam_tabla){
		return true;
	}else return false;
}
