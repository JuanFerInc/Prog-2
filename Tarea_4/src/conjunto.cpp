#include "stdio.h"
#include "string.h"
#include "../include/conjunto.h"
#include "../include/binario.h"
#include "../include/cadena.h"
#include "../include/info.h"

struct rep_conjunto{
	binario_t dato;
};

typedef struct rep_conjunto *conjunto_t;


conjunto_t crear_conjunto(){
	conjunto_t c = new rep_conjunto;
	c->dato = crear_binario();
	return c;
}

conjunto_t construir_conjunto(cadena_t cad){
	conjunto_t c = new rep_conjunto;
	c->dato = crear_balanceado(cad);
	return c;
}

bool incluir(info_t i, conjunto_t &c){
	return insertar_en_binario(i,c->dato);
}


static void copiar_conjunto(binario_t &copia, binario_t original){
	if(!es_vacio_binario(original)){
		info_t nuevo_info = copia_info(raiz(original));	//O(log n)
		
		if(!insertar_en_binario(nuevo_info,copia)){ 	//O(log n)
			liberar_info(nuevo_info);					//O(1)
		}
		copiar_conjunto(copia,izquierdo(original)); 	
		copiar_conjunto(copia,derecho(original));		
	}
}

conjunto_t union_conjunto(conjunto_t s1, conjunto_t s2){
  	conjunto_t c = crear_conjunto();		
  	copiar_conjunto(c->dato,s1->dato); 		//O(n1.log n1)
  	copiar_conjunto(c->dato,s2->dato);		//O(n2.log n)

  	return c;
}

conjunto_t interseccion(conjunto_t c1, conjunto_t c2){
	//inicio las cadenavs que voy a utilisar
	cadena_t cad1, cad2;
	cadena_t res = crear_cadena();
	
	// paso los arboles a cadenas;
	cad1 = linealizacion(c1->dato);
	cad2 = linealizacion(c2->dato);
	
	//inicio los localisadores de las 2 cadeans con los elementos
	localizador_t loc1,loc2;
	loc1 = inicio_cadena(cad1);
	loc2 = inicio_cadena(cad2);

	while( es_localizador(loc1) && es_localizador(loc2)){
		
		if(strcmp(frase_info(info_cadena(loc1,cad1)),frase_info(info_cadena(loc2,cad2))) == 0){
			insertar_al_final(copia_info(info_cadena(loc1,cad1)),res);
			loc1 = siguiente(loc1,cad1);
			loc2 = siguiente(loc2,cad2);

		}else if(strcmp(frase_info(info_cadena(loc1,cad1)),frase_info(info_cadena(loc2,cad2))) < 0){
			loc1 = siguiente(loc1,cad1);
		}else{//seria el caso en el que el elemento de cad 2 es menor al elemento de cad 1
			loc2 = siguiente(loc2,cad2);
		}
			}

	liberar_cadena(cad1);
	liberar_cadena(cad2);

	conjunto_t res_conjunto = crear_conjunto();
	res_conjunto->dato = crear_balanceado(res);
	liberar_cadena(res);

	return res_conjunto;
}

conjunto_t diferencia(conjunto_t c1, conjunto_t c2){
 	//inicio las cadenavs que voy a utilisar
	cadena_t cad1, cad2;
	cadena_t res = crear_cadena();
	
	// paso los arboles a cadenas;
	cad1 = linealizacion(c1->dato);
	cad2 = linealizacion(c2->dato);
	
	//inicio los localisadores de las 2 cadeans con los elementos
	localizador_t loc1,loc2;
	loc1 = inicio_cadena(cad1);
	loc2 = inicio_cadena(cad2);
	if(!es_localizador(loc2) && es_localizador(loc1)){
			while(es_localizador(loc1)){
				insertar_al_final(copia_info(info_cadena(loc1,cad1)),res);
				loc1 = siguiente(loc1,cad1);
			}

	}else{
		while( es_localizador(loc1) && es_localizador(loc2)){
			
			if(strcmp(frase_info(info_cadena(loc1,cad1)),frase_info(info_cadena(loc2,cad2))) == 0){
				//insertar_al_final(copia_info(info_cadena(loc1,cad1)),res);
				loc1 = siguiente(loc1,cad1);
				loc2 = siguiente(loc2,cad2);

			}else if(strcmp(frase_info(info_cadena(loc1,cad1)),frase_info(info_cadena(loc2,cad2))) < 0){
				insertar_al_final(copia_info(info_cadena(loc1,cad1)),res);
				loc1 = siguiente(loc1,cad1);
			}else{//seria el caso en el que el elemento de cad 2 es menor al elemento de cad 1

				loc2 = siguiente(loc2,cad2);
			}
		}
			
	}
			
	liberar_cadena(cad1);
	liberar_cadena(cad2);

	conjunto_t res_conjunto = crear_conjunto();
	res_conjunto->dato = crear_balanceado(res);
	liberar_cadena(res);

	return res_conjunto;
}

void excluir(char *t, conjunto_t &c){
	remover_de_binario(t,c->dato);
}

void liberar_conjunto(conjunto_t &c){
	if(!(es_vacio_binario(c->dato))){
		liberar_binario(c->dato);		
	}
	delete(c);
}

bool pertenece_conjunto(char *t, conjunto_t c){
  	if (es_vacio_binario(c->dato)){
		return false;
	}else if(strcmp(t,frase_info(raiz(c->dato))) < 0) {
		return buscar_subarbol(t,izquierdo(c->dato));
	}else if(strcmp(t,frase_info(raiz(c->dato))) > 0){
		return buscar_subarbol(t,derecho(c->dato));
	} else {
		return true;
	}
}

bool es_vacio_conjunto(conjunto_t c){
  if(es_vacio_binario(c->dato)){
  	return true;
  }else return false;
}

iterador_t iterador_conjunto(conjunto_t c){
	iterador_t res = crear_iterador();						 // O(1)
	cadena_t cad = linealizacion(c->dato);					 // O(n)
	localizador_t loc = inicio_cadena(cad); 				 // O(1)
	char *frase;											 // O(1)
	unsigned int length;							 
	while(es_localizador(loc)){								 // O(n)

		length = strlen(frase_info(info_cadena(loc,cad)));   // O(?)
		frase = new char[length+1];						     // O(1)
		strcpy(frase,frase_info(info_cadena(loc,cad)));	     // O(?)
		
		agregar_a_iterador(frase,res);						 // O(1)
		loc = siguiente(loc,cad);							 // O(1)
	}

	liberar_cadena(cad);									 // O(n)


  	return res;
}
