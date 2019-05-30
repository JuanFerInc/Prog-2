#include "../include/info.h"
#include "../include/cadena.h"
#include "../include/binario.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

struct rep_binario {
	info_t dato;
	rep_binario *izq;
	rep_binario *der;
};

binario_t crear_binario(){
	return NULL;
}

bool insertar_en_binario(info_t i, binario_t &b){
	if(b == NULL){
		binario_t leaf = new rep_binario;
		leaf->dato = i;
		leaf->izq = leaf->der = NULL;
		b = leaf;
		return true;
	}else if (strcmp(frase_info(i),frase_info(b->dato)) < 0 ){
		return insertar_en_binario(i,b->izq);	
	}else if(strcmp(frase_info(i),frase_info(b->dato)) > 0){
		return insertar_en_binario(i,b->der);
	}else {return false;};
}



info_t remover_mayor(binario_t &b){
	info_t res;
	if (b->der == NULL){
		res = b->dato;
		binario_t izq = b->izq;
		delete(b);
		b = izq;
	}else{
		res = remover_mayor(b->der);
	}
	return res;
}

bool remover_de_binario(const char *t, binario_t &b){
if (es_vacio_binario(b)){
		return false;
	}else if(strcmp(t,frase_info(b->dato)) < 0) {
		return remover_de_binario(t,b->izq);
	}else if(strcmp(t,frase_info(b->dato)) > 0){
		return remover_de_binario(t,b->der);
	} else {
		// al encontrar necesitamos remplasarlo por el valor mas
		// grande del subarbol izquierdo
		if(b->izq == NULL){
			if(es_vacio_binario(b->der)){
			 	liberar_binario(b);
			 	 b = NULL;
			}else {
			binario_t aux = b->der;
			liberar_info(b->dato);
			b->dato = aux->dato;
			b->der = aux->der;
			b->izq = aux->izq;
			delete(aux);
		}
		}else{
			info_t mayor = remover_mayor(b->izq);
			liberar_info(b->dato);
			b->dato = mayor;	
		}
		return true;
	}
}

void liberar_binario(binario_t &b){
	if(!(es_vacio_binario(b))){
		liberar_binario(b->izq);
		liberar_binario(b->der);
		liberar_info(b->dato);
		delete(b);
	}
}

bool es_vacio_binario(binario_t b){
	if (b == NULL){
		return true;
	}else return false;
}
static nat distancia(nat a, nat b){
	if(a < b){
		return (b-a);
	}else return (a-b);

}

bool es_AVL(binario_t b){
	if(es_vacio_binario(b)){
		return true;
	}else if ((distancia(altura_binario(b->izq) , altura_binario(b->der)) < 2)) {
		return true * es_AVL(b->izq) * es_AVL(b->der);
	} else return false;
	//return true * (distancia(altura_binario(b->izq) , altura_binario(b->der)) < 2);
}

info_t raiz(binario_t b){
	info_t res = b->dato;
	return res;
}

binario_t izquierdo(binario_t b){
	return b->izq;
}

binario_t derecho(binario_t b){
	return b->der;
}

binario_t buscar_subarbol(const char *t, binario_t b){
	if (es_vacio_binario(b)){
		return NULL;
	}else if(strcmp(t,frase_info(b->dato)) < 0) {
		return buscar_subarbol(t,b->izq);
	}else if(strcmp(t,frase_info(b->dato)) > 0){
		return buscar_subarbol(t,b->der);
	} else {
		return b;
	}
}

static nat max(nat a, nat b){
	if (a > b){
		return a;
	}else return b;
}

nat altura_binario(binario_t b){
	if(es_vacio_binario(b)){
		return 0;
	}else return (1 + max(altura_binario(b->izq), altura_binario(b->der)));
}

nat cantidad_binario(binario_t b){
	if(es_vacio_binario(b)){
		return 0;
	}else return (cantidad_binario(b->izq) + cantidad_binario(b->der) + 1);
}

info_t kesimo_en_binario(nat k, binario_t b){
	// priemro calculamos la cantidad de nodos del sub-arbol izquierdo que tiene como raiz al nodo
	// en el que estamos parado
	
	nat altura  = cantidad_binario(b->izq)+1;
	//casos extremos
	if((k == 0) || es_vacio_binario(b)){
		return NULL;
	}
	// si la cantidad de nodos es igual al valor k entonses es el nodo en el que estamos parado
	else if( k == altura){
		return b->dato;
	}
	// si la cantidad de nodos es menor a k entoneses sabemos que se encuentra en el sub-arbol derecho
	// le restamos la cantidad de nodos del sub-arbol izquierdo
	else if(k > altura){
		return kesimo_en_binario(k-altura,b->der);
	}
	// en ultimo caso tenemos que k es menor a la cantidad de nodos del sub-arbol izquierdo
	// sabemos que se e en el sub-arbol izquierdo
	else return kesimo_en_binario(k,b->izq);
}

static cadena_t aux_linealizacion(binario_t b, cadena_t &cad){
	if (es_vacio_binario(b)){
		return NULL;
	}else {
		aux_linealizacion(b->der,cad);
		info_t aux_info = copia_info(b->dato);
		if(es_vacia_cadena(cad)){
			insertar_al_final(aux_info,cad);
		}else{
			insertar_antes(aux_info,inicio_cadena(cad),cad);	
		}
		
		aux_linealizacion(b->izq,cad);

	}
	return cad;
}

cadena_t linealizacion(binario_t b){
	cadena_t cad = crear_cadena();
	aux_linealizacion(b,cad);
	return cad;
}
// funcion auxiliar utilisada por filtrado_aux que retorna el dato numero del nodo mas grande
static info_t maximo_nodo_info(int clave, binario_t b){
	if(es_vacio_binario(b)){
		return NULL;
	}else if(numero_info(b->dato) >= clave){
		return NULL;
	}else if(es_vacio_binario(b->der)){
		return copia_info(b->dato);
	}else if(numero_info(b->der->dato) >= clave){
		return copia_info(b->dato);
	}else return maximo_nodo_info(clave,b->der);
}

// cada nodo que cumpla la condicion es agregado a res
// si el nodo no complue la condicion, se agrega el mayor nodo del sub-arbol izquierd
//  para mantener la misma estructura que el arbol binario origianl

static void filtrado_aux(int clave, binario_t b, binario_t &res){
	info_t aux_info = NULL;
	
	if(!es_vacio_binario(b)){												//si el arbol es vacio se hace nada
		if(numero_info(b->dato) < clave){									//primer se mira si el nodo cumple la condicion
			aux_info = copia_info(b->dato);								 	//si este cumple, se lo agrega al arbol
			if(insertar_en_binario(aux_info,res) == false){					// aseguramos que el nodo fue insertado, de lo contrario se borra la informacion
					liberar_info(aux_info);
				}
			filtrado_aux(clave,b->izq,res);
			filtrado_aux(clave,b->der,res);
		}else if(numero_info(b->dato) >= clave) {							// si el nodo en el que estamos parado no cumple la condicion buscamos el mas grande en el sub-arbol izquierdo
			if(b->izq != NULL){												// que cumpla la condicion
				aux_info = maximo_nodo_info(clave,b->izq);
				if(aux_info != NULL){
					if(insertar_en_binario(aux_info,res) == false){			// seguramos que el nodo se aya insertado, de lo contrario se borra la informacion obtenida
						liberar_info(aux_info);
					}
				}
			}
			filtrado_aux(clave,b->izq,res);									// llamado recursivo izquierdo
			filtrado_aux(clave,b->der,res);									// llamado recursivo derecho
		}
	}

}

binario_t filtrado(int clave, binario_t b){
	binario_t res = crear_binario();
	filtrado_aux(clave,b,res);
	return res;
}

static void aux_imprimir_binario(int altura, binario_t b){
	if (es_vacio_binario(b)){
	}else {
		aux_imprimir_binario(altura+1,b->der);
		for(int i = 0; i<altura; i++){
			printf("-");
		}
		printf("(%d,%s)\n", numero_info(b->dato),frase_info(b->dato));
		aux_imprimir_binario(altura+1,b->izq);
	}
}

void imprimir_binario(binario_t b){
	printf("\n");
	// utilisando una funcion aux que reciva comom parametro adicional un int poder mantener
	// un control de la altura en la que se encuentra la funcion para lograr imprimir 
	// la cantidad de caracteres que correspondan
	aux_imprimir_binario(0,b);
}