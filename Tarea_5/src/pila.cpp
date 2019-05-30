/* 48191081 */
#include "../include/pila.h"
#include "stdio.h"

struct pila_nodo{
	char *frase;
	pila_nodo* sig;
};

struct rep_pila{
	pila_nodo* elem;
	int tam;
	int actual;
};



typedef struct rep_pila *pila_t;
typedef struct pila_nodo *pila_n;

pila_t crear_pila(int tamanio){
  pila_t pila = new rep_pila;
  pila->elem = NULL;
  pila->tam = tamanio;
  pila->actual = 0;
  return pila;
}

void apilar(char *t, pila_t &p){
	if(!es_llena_pila(p)){
		pila_n nuevo = new pila_nodo;
		nuevo->frase = t;
		nuevo->sig = p->elem;
		p->elem = nuevo;
		p->actual++;	
	}
}

void desapilar(pila_t &p){
	if(p->elem != NULL){
		pila_n aux = p->elem;
		p->elem = p->elem->sig;
		p->actual--;
		delete[] aux->frase;
		delete aux;
	}
	 
}

void liberar_pila(pila_t &p){
	while(!es_vacia_pila(p)){
		desapilar(p);
	}
	delete(p);
}

bool es_vacia_pila(pila_t p){
  if(p->actual == 0){
  	return true;
  }else return false;
}

bool es_llena_pila(pila_t p){
  if(p->actual >= p->tam){
  	return true;
  }else return false;
}

char *cima(pila_t p){
    return p->elem->frase;
}