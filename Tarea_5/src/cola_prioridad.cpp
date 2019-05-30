/* 48191081 */
#include "stdio.h"
#include "../include/cola_prioridad.h"

struct info_elems{
	info_t elem;
	prio_t p;
};
struct loc_elems{
	bool esta;
	unsigned int pos;
};

struct rep_cola_prioridad{
	loc_elems *arr_prio;
	info_elems *arr_info;
	prio_t max_p;
	unsigned int tam_max;
	unsigned int cant_elem;
};

static void move_closer_leaf(unsigned int pos, cola_prioridad_t &cp){
	bool insertado = false;
	info_elems aux;
	
	aux.elem = cp->arr_info[pos].elem;
	aux.p = cp->arr_info[pos].p;

	cp->arr_info[pos].elem = NULL;

	unsigned int pos_aux;
	while(((pos*2 <= cp->tam_max) && (pos*2+1 <= cp->tam_max)) && ( (cp->arr_info[pos*2].elem != NULL) || (cp->arr_info[(pos*2)+1].elem != NULL) ) && (!insertado) ){
		// miro si existe el hijo derecho, de lo contrario solo esta el hijo izquierdo
		if(cp->arr_info[pos*2+1].elem != NULL){
			if(cp->arr_info[pos*2].p > cp->arr_info[pos*2+1].p){
				pos_aux = pos*2+1;
			}else{
				pos_aux = pos*2;
			}
		// en este caso no existe el hijo derecho, como entro al while tiene que existir el hijo izquierdo
		}else{
			pos_aux = pos*2;
		}
		// miro si la prioridad del hijo elegido es menor a la pa medida que sigma la campana se contrae, cuando sigma crerioridad del nodo que busco mover
		// si es menor muevo el hijo a la posicion del nodo actual 
		if(cp->arr_info[pos_aux].p < aux.p){
			cp->arr_info[pos].elem = cp->arr_info[pos_aux].elem;
			cp->arr_info[pos_aux].elem = NULL;
			cp->arr_info[pos].p = cp->arr_info[pos_aux].p;
			cp->arr_prio[cp->arr_info[pos].p].pos = pos;
			pos = pos_aux;
		}
		//de lo contrario inserto en el lugar que estoy posicionado
		else{
			cp->arr_info[pos].elem = aux.elem;
			cp->arr_info[pos].p = aux.p;
			cp->arr_prio[aux.p].esta = true;
			cp->arr_prio[aux.p].pos = pos;
			insertado = true;
		}
	}
	if(!insertado){
		cp->arr_info[pos].elem = aux.elem;
		cp->arr_info[pos].p = aux.p;
		cp->arr_prio[aux.p].esta = true;
		cp->arr_prio[aux.p].pos = pos;
	}

}

cola_prioridad_t crear_cp(unsigned int tamanio, prio_t max_prioridad){
	cola_prioridad_t cola = new rep_cola_prioridad;
	cola->arr_info = new info_elems[tamanio+1];
	cola->arr_prio = new loc_elems[max_prioridad+1];
	cola->cant_elem = 0;
	cola->tam_max = tamanio;
	cola->max_p = max_prioridad;

	for(unsigned int i = 0; i < tamanio+1; i++){
		cola->arr_info[i].elem = NULL;
	}
	for(unsigned int i = 0; i < max_prioridad+1; i++){
		cola->arr_prio[i].esta = false;
		cola->arr_prio[i].pos = 0;

	}
	return cola;
}


void insertar_en_cp(info_t i, prio_t p, cola_prioridad_t &cp){
	if(cp->cant_elem == 0){
		cp->arr_info[1].elem = i;
		cp->arr_info[1].p = p;
		cp->arr_prio[p].esta = true;
		cp->arr_prio[p].pos = 1;
		cp->cant_elem++;
	}else{
		  bool insertado = false;
		  unsigned int pos = cp->cant_elem+1;
		  while (!insertado){
		  	if((pos != 1) && (cp->arr_info[pos/2].p > p)){
		  		cp->arr_info[pos].elem = cp->arr_info[pos/2].elem;
		  		cp->arr_info[pos].p = cp->arr_info[pos/2].p;
		  		cp->arr_prio[cp->arr_info[pos].p].pos = pos;
		  		pos = pos/2;

		  	}else{
		  		cp->arr_info[pos].elem = i;
		  		cp->arr_info[pos].p = p;
		  		cp->arr_prio[p].esta = true;
		  		cp->arr_prio[p].pos = pos;
		  		cp->cant_elem++;
		  		insertado = true;
			}
		}
	}
}


void priorizar(prio_t p, cola_prioridad_t &cp){
	bool insertado = false;
	unsigned int pos = cp->arr_prio[p].pos;
	info_elems aux;

	// guardamos la informacion en un aux con el nuevo valor de prioridad
	aux.elem = cp->arr_info[pos].elem;
	aux.p = cp->arr_info[pos].p/2;

	// actualisamos el arreglo de prioridads
	cp->arr_prio[p].esta = false;
	cp->arr_prio[p].pos = 0;

	//inserto
		while (!insertado){
		if((pos > 1) && (cp->arr_info[pos/2].p > aux.p)){
			cp->arr_info[pos].elem = cp->arr_info[pos/2].elem;
			cp->arr_info[pos].p = cp->arr_info[pos/2].p;
			cp->arr_prio[cp->arr_info[pos].p].pos = pos;
			pos = pos/2;
	  	}else{
	  		cp->arr_info[pos].elem = aux.elem;
	  		cp->arr_info[pos].p = aux.p;
	  		cp->arr_prio[aux.p].esta = true;
	  		cp->arr_prio[aux.p].pos = pos;
	  		insertado = true;
		}
	}
}

void despriorizar(prio_t p, cola_prioridad_t &cp){
	unsigned int pos = cp->arr_prio[p].pos;
	
	cp->arr_prio[p].esta = false;
	cp->arr_prio[p].pos = 0;

	cp->arr_info[pos].p = (cp->arr_info[pos].p + cp->max_p) / 2;
	move_closer_leaf(pos,cp);


}

void eliminar_prioritario(cola_prioridad_t &cp){
	// borrado logico del primer elemento del heap
	cp->arr_prio[cp->arr_info[1].p].esta = false;
	cp->arr_prio[cp->arr_info[1].p].pos = 0;
	// libero la memorioa del elemento del pimer nodo
	liberar_info(cp->arr_info[1].elem);
	cp->arr_info[1].elem = NULL;
	unsigned int ultimo = cp->cant_elem;
	cp->cant_elem--;

	if(cp->cant_elem > 0){
		// pongo el ultimo nodo en el primer lugar y lo borro del ultimo lugar
		cp->arr_info[1].elem = cp->arr_info[ultimo].elem;
		cp->arr_info[ultimo].elem = NULL;
		cp->arr_info[1].p = cp->arr_info[ultimo].p;
		cp->arr_info[ultimo].p = 0;
		//actualiso la cant_elemidad de elementos disponible	
		move_closer_leaf(1,cp);	
	}
}

void liberar_cp(cola_prioridad_t &cp){
	for (unsigned int i = 1; i <= cp->cant_elem; i++  ){
		liberar_info(cp->arr_info[i].elem);
	}
	delete[] cp->arr_info;
	delete[] cp->arr_prio;
	delete cp;
	
}


bool es_vacia_cp(cola_prioridad_t cp){
	if(cp->cant_elem == 0){
		return true;
	}else{
		return false;
	}
}


bool es_llena_cp(cola_prioridad_t cp){
	if(cp->cant_elem == cp->tam_max){
		return true;
	}else {
		return false;
	}
}


bool hay_prioridad(prio_t p, cola_prioridad_t cp){
	if(p > cp->max_p){
		return false;
	}else{
		return cp->arr_prio[p].esta;	
	}
	
}


info_t prioritario(cola_prioridad_t cp){
	return cp->arr_info[1].elem;
}


prio_t prioridad_prioritario(cola_prioridad_t cp){
	return cp->arr_info[1].p;
}


prio_t max_prioridad(cola_prioridad_t cp){
	return cp->max_p;
}
