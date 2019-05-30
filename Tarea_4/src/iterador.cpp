#include "../include/iterador.h"
#include "stdio.h"
#include "../include/cadena.h"

struct rep_iterador{
	cadena_t cad;
	localizador_t pos;
};

typedef struct rep_iterador *iterador_t;

iterador_t crear_iterador(){
  iterador_t t = new rep_iterador;
  t->cad = crear_cadena();
  t->pos = NULL;
  return t;
}

void agregar_a_iterador(char *t, iterador_t &i){
 	info_t info = crear_info(0,t);
 	insertar_al_final(info,i->cad);
}

void reiniciar_iter(iterador_t &i){
  i->pos = inicio_cadena(i->cad);
}

void avanzar_iter(iterador_t &i){
	if(!(i->pos == NULL)){
	 	i->pos = siguiente(i->pos,i->cad);
	 }else {
	 	i->pos = NULL;
	 }
}	

char *actual_iter(iterador_t &i){
	return frase_info(info_cadena(i->pos,i->cad));
}

bool hay_siguiente_en_iter(iterador_t i){
 	localizador_t aux = siguiente(i->pos,i->cad);
 	if(aux == NULL){
 		return false;
 	}else return true;
}

bool esta_definida_actual(iterador_t i){
 	return es_localizador(i->pos);
}

cadena_t iterador_a_cadena(iterador_t i){
  cadena_t res;
  if(!es_vacia_cadena(i->cad)) {
  	res = segmento_cadena(inicio_cadena(i->cad), final_cadena(i->cad), i->cad);
  }else{
  	res = crear_cadena();
  }

  return res;
}

void liberar_iterador(iterador_t &i){
	liberar_cadena(i->cad);
	delete(i);	
	
	
}
