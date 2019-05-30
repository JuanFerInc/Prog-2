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

nat aux_es_AVL( binario_t b, bool &soy_AVL){
	if (es_vacio_binario(b)){
			return 0;
	}else {
		nat l = aux_es_AVL(b->izq,soy_AVL);	// calculando la altura del arbol izquierdo
		nat r = aux_es_AVL(b->der,soy_AVL);	// calcula la altura del arbol derecho

		if(distancia(l,r) >= 2 ){			//verifica que sea AVL, de lo contrario 
			soy_AVL = false;
		} 
		return (max(l,r)+1);				//retorna la altura del arbol izq o der + el nodo actual
	}
}

bool es_AVL(binario_t b){
 bool es = true;
 aux_es_AVL(b,es);
 return es;
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

/*
binario_t crear_balanceado_aux(nat largo,nat inicio,localizador_t loc_in, cadena_t cad){
	localizador_t mitad_cad = loc_in;														//O(1)
	binario_t res = crear_binario();												//O(1)
	int half;
	if(largo >= 1) {																//O(1)
		if(largo == 1){																//O(1)
			info_t copia = copia_info(info_cadena(mitad_cad,cad)); 					//O(O(1 O(1(O(1))))
			insertar_en_binario(copia,res);											//O(1)

		}else{
			// encuentro la mitad de la cadena
			nat mitad_n, largo_primer_mitad, largo_segunda_mitad;					//O(1)
			if(largo % 2 == 0){														//O(1)
				half = (largo/2)+1;												//O(1)
				mitad_n = half+inicio-1;												//O(1)
				largo_primer_mitad = half;											//O(1)
				largo_segunda_mitad = half-1;										//O(1)
			}else{
				half = (largo+1) /2;
				mitad_n = half + inicio-1; 									//O(1)
				largo_primer_mitad = ((largo+1)/2)-1;								//O(1)
				largo_segunda_mitad = ((largo+1)/2)-1;								//O(1)
			}
			for(int i = 1;i<= half; i++){
				mitad_cad = siguiente(mitad_cad,cad);
			}
			//mitad_cad = kesimo(mitad_n,cad); // este nodo va a ser la raiz			//O(n/2)
			// hago una copia del elemento del medio  y inserto en un arbol binario
			info_t copia = copia_info(info_cadena(mitad_cad,cad)); 					//O(1)
			insertar_en_binario(copia,res);											//O(1)

			res->izq = crear_balanceado_aux(largo_primer_mitad,inicio,loc_in,cad);			
			res->der = crear_balanceado_aux(largo_segunda_mitad,(mitad_n+1),siguiente(mitad_cad,cad),cad);		
		}
	}
	return res;																		//O(1)
}

binario_t crear_balanceado(cadena_t cad){ 	
	localizador_t loc = inicio_cadena(cad);										
	nat largo = largo_cadena(loc,cad);
	binario_t res = crear_binario();
 	return res = crear_balanceado_aux(largo,1,loc, cad);
}
*/
/*
THIS IS THE BACKUP

binario_t crear_balanceado_aux (nat largo,cadena_t cad){
	if(es_vacia_cadena(cad)){
		return NULL;
	}else{
		// encuentro la mitad de la cadena
		nat mitad_n, largo_primer_mitad, largo_segunda_mitad;
		if(largo % 2 == 0){
			mitad_n = (largo/2)+1;
			largo_primer_mitad = mitad_n-1;
			largo_segunda_mitad = mitad_n -2;
		}else{
			mitad_n = (largo +1 ) /2; 
			largo_primer_mitad = mitad_n-1;
			largo_segunda_mitad = mitad_n-1;
		}
		localizador_t mitad_cad = kesimo(mitad_n,cad); // este nodo va a ser la raiz			//O(n/2)
		
		// hago una copia del elemento del medio  y inserto en un arbol binario_t			
		info_t copia = copia_info(info_cadena(mitad_cad,cad)); 									//O(1)
		binario_t res = crear_binario();														//O(1)
		insertar_en_binario(copia,res);															//O(1)
		
		//divido la cadena en dos partes
		cadena_t primera_mitad;																	//O(1)
		cadena_t segunda_mitad;																	//O(1)

		if(largo > 2){
			primera_mitad = segmento_cadena(inicio_cadena(cad), anterior(mitad_cad,cad),cad);	//O(n/2)
			segunda_mitad = segmento_cadena(siguiente(mitad_cad,cad), final_cadena(cad),cad);	//O(n/2)
		}else if(largo == 2){
			primera_mitad = segmento_cadena(inicio_cadena(cad), anterior(mitad_cad,cad),cad);	//O(n/2)
			segunda_mitad = crear_cadena();			//O(1)
		}
		else {
			primera_mitad = crear_cadena();			//O(1)
			segunda_mitad = crear_cadena();			//O(1)
		}
		
		// llamado recusivo en las mitades
		res->izq = crear_balanceado_aux(largo_primer_mitad,primera_mitad);						
		res->der = crear_balanceado_aux(largo_segunda_mitad,segunda_mitad);
		
		liberar_cadena(primera_mitad);
		liberar_cadena(segunda_mitad);
		return res;
	}
}

binario_t crear_balanceado(cadena_t cad){
	localizador_t loc = inicio_cadena(cad); 	
	nat largo = largo_cadena(loc,cad); 			
	return crear_balanceado_aux(largo, cad);
}
*/

binario_t crear_balanceado_aux (nat largo,cadena_t &cad){
	if(es_vacia_cadena(cad)){
		liberar_cadena(cad);
		return NULL;
	}else{
		// encuentro la mitad de la cadena
		nat mitad_n, largo_primer_mitad, largo_segunda_mitad;
		if(largo % 2 == 0){
			mitad_n = (largo/2)+1;
			largo_primer_mitad = mitad_n-1;
			largo_segunda_mitad = mitad_n -2;
		}else{
			mitad_n = (largo +1 ) /2; 
			largo_primer_mitad = mitad_n-1;
			largo_segunda_mitad = mitad_n-1;
		}
		localizador_t mitad_cad = kesimo(mitad_n,cad); // este nodo va a ser la raiz			//O(n/2)
		
		// hago una copia del elemento del medio  y inserto en un arbol binario_t			
		info_t copia = copia_info(info_cadena(mitad_cad,cad)); 									//O(1) 
		binario_t res = crear_binario();														//O(1)
		insertar_en_binario(copia,res);															//O(1)
		
		//divido la cadena en dos partes
		cadena_t primera_mitad;																	//O(1)
		cadena_t segunda_mitad;																	//O(1)

		if(largo > 2){
			primera_mitad = separar_segmento(inicio_cadena(cad), anterior(mitad_cad,cad),cad);	//O(n/2)
			segunda_mitad = separar_segmento(siguiente(mitad_cad,cad), final_cadena(cad),cad);	//O(n/2)
		}else if(largo == 2){
			primera_mitad = separar_segmento(inicio_cadena(cad), anterior(mitad_cad,cad),cad);	//O(n/2)
			segunda_mitad = crear_cadena();			//O(1)
		}
		else {
			primera_mitad = crear_cadena();			//O(1)
			segunda_mitad = crear_cadena();			//O(1)
		}
		liberar_cadena(cad);
		// llamado recusivo en las mitades 
		res->izq = crear_balanceado_aux(largo_primer_mitad,primera_mitad);						
		res->der = crear_balanceado_aux(largo_segunda_mitad,segunda_mitad);
		
		return res;
	}
}



binario_t crear_balanceado(cadena_t cad){
	cadena_t cad_copia = segmento_cadena(inicio_cadena(cad),final_cadena(cad),cad);	//O(n)
	nat largo = largo_cadena(inicio_cadena(cad_copia),cad_copia); 					//O(n)
	
	binario_t res = crear_balanceado_aux(largo, cad_copia);
	//liberar_cadena(cad_copia);
	return res;
}
