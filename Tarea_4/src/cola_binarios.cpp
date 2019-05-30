#include "../include/cola_binarios.h"
#include "stdio.h"


struct rep_nodo_cola {
	binario_t arbol;
	rep_nodo_cola *sig;

};
struct rep_cola_binarios {
	rep_nodo_cola *egreso;
	rep_nodo_cola *ingreso;
};
typedef struct rep_cola_binarios *cola_binarios_t;
typedef struct rep_nodo_cola *nodo_cola;

cola_binarios_t crear_cola_binarios(){
	cola_binarios_t c = new rep_cola_binarios;
	c->ingreso = NULL;
	c->egreso = NULL;
	return c;
}

void encolar(binario_t b, cola_binarios_t &c){
	if(es_vacia_cola_binarios(c)){
		nodo_cola nodo = new rep_nodo_cola;
		nodo->arbol = b;
		nodo->sig = NULL;
		c->ingreso = nodo;
		c->egreso = nodo;
	}else{
		nodo_cola nodo = new rep_nodo_cola;
		nodo->arbol = b;
		nodo->sig = NULL;
		c->ingreso->sig = nodo;
		c->ingreso = nodo;
	}
}

void desencolar(cola_binarios_t &c){
	if(!es_vacia_cola_binarios(c)){
		
		nodo_cola aux = c->egreso;
		c->egreso = c->egreso->sig;
		if(c->egreso == NULL){
			c->ingreso = NULL;
		}
		delete(aux);
	
	}
}
static void liberar_cola_binarios_aux(cola_binarios_t &c){
		if( !es_vacia_cola_binarios(c)){
		nodo_cola aux = c->egreso;
		c->egreso = c->egreso->sig;
		if(c->egreso == NULL){
			c->ingreso = NULL;
		}
		delete aux;
		liberar_cola_binarios_aux(c);
	}
}
void liberar_cola_binarios(cola_binarios_t &c){
	liberar_cola_binarios_aux(c);
	delete(c);
}

bool es_vacia_cola_binarios(cola_binarios_t c){
	if((c->egreso == NULL) && (c->ingreso == NULL)){
		return true;
	}else return false;
}

binario_t frente(cola_binarios_t c){
	if(!es_vacia_cola_binarios(c)){
		return c->egreso->arbol;
	}else return NULL;
}
