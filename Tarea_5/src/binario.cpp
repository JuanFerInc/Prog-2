/* 48191081 */
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
		binario_t aux_b = NULL;
		if (b->izq == NULL){
			aux_b = b;
			b  = b->der;
			liberar_info(aux_b->dato);
			delete(aux_b);
		}else if(b->der == NULL){
			aux_b = b;
			b  = b->izq;
			liberar_info(aux_b->dato);
			delete(aux_b);
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


// retorna la distancia entre dos enter
static nat distancia(nat a, nat b){
	if(a < b){
		return (b-a);
	}else return (a-b);

}


// retorna el maximo entre dos numeros
static nat max(nat a, nat b){
	if (a > b){
		return a;
	}else return b;
}



bool aux_es_AVL(binario_t b, nat  &altura){
	nat altura_izq = 0;
	nat altura_der = 0;
	if(b == NULL){
		return true;
		altura = 0;
	}

	bool izq = aux_es_AVL(b->izq, altura_izq);
	bool der = aux_es_AVL(b->der,altura_der);

	altura = max(altura_izq,altura_der) +1;

	if(distancia(altura_izq,altura_der) >= 2){
		return false;
	}else return izq&&der;
	
}

bool es_AVL(binario_t b){
 nat altura = 0;
 return aux_es_AVL(b,altura);
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

info_t kesimo_en_binario_aux(nat k, nat &nodo,binario_t b){
	// si estoy en un nodo del extremo retorno null
	if((k == 0)|| (b == NULL)){
		return NULL;
	}else{
		// tengo que posicionarme en el ultimo nodo
		info_t res = kesimo_en_binario_aux(k,nodo,b->izq);
		// cuento el nodo
		nodo++;
		// posible que nodo sol venga de otro nodo cuando recorri el lado izquierdo
		// o que el nodo en el que estamos sea solucion
		// de lo contrario verificamos el subarbol derecho
		if((res != NULL)){
			return res;
		}else if(k == nodo){
			return b->dato;
		}else{ 
		return kesimo_en_binario_aux(k,nodo,b->der);		
		}
		
	}
	

}

info_t kesimo_en_binario(nat k, binario_t b){
nat nodo = 0;
return kesimo_en_binario_aux(k,nodo,b);
	

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
		return maximo_nodo_info(clave,b->der);
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

// Dado una cadena y un ocalizador de la cadena, retorna el largo de la cadena
// O(n)
nat largo_cadena(localizador_t loc, cadena_t cad){
	nat largo = 0;
	while(es_localizador(loc)){
		loc = siguiente(loc,cad);
		largo++;
	}
	return largo;
}


binario_t crear_balanceado_aux (nat largo,cadena_t &cad){
	if(largo <= 0){
		return NULL;
	}else {

		binario_t res = new rep_binario;
		res->izq = crear_balanceado_aux(largo/2,cad);

		localizador_t loc = inicio_cadena(cad);
		info_t copia = copia_info(info_cadena(loc,cad));
		res->dato = copia;
		remover_de_cadena(loc,cad);
		res->der = crear_balanceado_aux(largo - (largo/2)-1,cad);
		return res;
	}
}



binario_t crear_balanceado(cadena_t cad){
	cadena_t cad_copia = segmento_cadena(inicio_cadena(cad),final_cadena(cad),cad);
	nat largo = largo_cadena(inicio_cadena(cad_copia),cad_copia); 					
	binario_t res = crear_balanceado_aux(largo, cad_copia);
	liberar_cadena(cad_copia);
	return res;
}