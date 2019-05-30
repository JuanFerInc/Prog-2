#include "stdio.h"
#include "string.h"
#include "../include/uso_tads.h"
#include "../include/cadena.h"
#include "../include/info.h"
#include "../include/iterador.h"
#include "../include/cola_binarios.h"

//recibe un puntero a una frase, la copia y agrega en el iterador
static void ACAI(char *frase, iterador_t &res){

	int tam = strlen(frase);
	char *copia = new char[tam+1];
	strcpy(copia,frase);
	agregar_a_iterador(copia,res);
}

iterador_t inverso_de_iter(nat a, nat b, iterador_t it){
	//inicialiso las variables necesaria
	cadena_t cad_a_copiar;
	cadena_t cad = iterador_a_cadena(it);
	localizador_t desde,hasta,inicio;
	iterador_t res = crear_iterador();



	//posiciono iteradors
	desde = kesimo(a,cad);
	hasta = kesimo(b,cad);

	//considero caso de borde e inserto
	if(es_localizador(desde)){
		if(!es_localizador(hasta)){
			hasta = final_cadena(cad);
		}
		// genero una copia de los elementos que van y posicion el localisador al final
		cad_a_copiar = segmento_cadena(desde,hasta,cad);
		inicio = final_cadena(cad_a_copiar);

		//mientras existan elementos en la cadena los copis desde final hasta inicio
		while(es_localizador(inicio)){

			ACAI(frase_info(info_cadena(inicio,cad_a_copiar)),res);
			inicio = anterior(inicio,cad);
			
		}
		//libero memoria solo sii fue inicialisada
		liberar_cadena(cad_a_copiar);
	}

	liberar_cadena(cad);
	
	//posiciones los iter al inicio
	reiniciar_iter(it);
	reiniciar_iter(res);
	return	res;
}

iterador_t rango_en_conjunto(char *primero, char *ultimo, conjunto_t c){
	
	//inicialiso la memoria necesaria
	iterador_t it = iterador_conjunto(c);
	cadena_t cad = iterador_a_cadena(it);
	localizador_t loc = final_cadena(cad);

	iterador_t res = crear_iterador();

	//busco el primer elemento que sea menor o igual al *ultimo
	while((es_localizador(loc)) && ( strcmp(frase_info(info_cadena(loc,cad)),ultimo) > 0 )){
		loc = anterior(loc,cad);
	}
	while( (es_localizador(loc)) && ( strcmp(frase_info(info_cadena(loc,cad)),primero)  >= 0 ) ) {
		ACAI(frase_info(info_cadena(loc,cad)),res);
		loc = anterior(loc,cad);

	}
	

	//liber la memoria solicitada
	liberar_iterador(it);
	liberar_cadena(cad);

	reiniciar_iter(res);
	return res;
}

bool esta_ordenada_por_frase(cadena_t cad){
  bool creo_que_esta_ordenado_de_forma_lexicografica = true;
  if(!es_vacia_cadena(cad)){
  	localizador_t actual,anterior;
  	actual = inicio_cadena(cad);
  	anterior = inicio_cadena(cad);
  	actual = siguiente(actual,cad);


	while(es_localizador(actual) && (creo_que_esta_ordenado_de_forma_lexicografica) ){
		if(strcmp(frase_info(info_cadena(anterior,cad)),frase_info(info_cadena(actual,cad))) >= 0){
	  		creo_que_esta_ordenado_de_forma_lexicografica = false;
	  	}
	  	anterior = siguiente(anterior,cad);
	  	actual = siguiente(actual,cad);
	}

  }
    return creo_que_esta_ordenado_de_forma_lexicografica;
}

static void imprimir_por_niveles_aux(cola_binarios_t &c ,int impnodos){
	if(!es_vacia_cola_binarios(c)){
		int nodo = 0;
	
		iterador_t iter = crear_iterador();
		while(impnodos > 0){
			if(!es_vacio_binario(izquierdo(frente(c)))){
				encolar(izquierdo(frente(c)),c);
				nodo++;
			}
			if(!es_vacio_binario(derecho(frente(c)))){
				encolar(derecho(frente(c)),c);
				nodo++;
			}
				ACAI(frase_info(raiz(frente(c))), iter);
				desencolar(c);
				impnodos--;
		}
		//llamado recursivo
		imprimir_por_niveles_aux(c,nodo);
		
		//imprimo los niveles
		reiniciar_iter(iter);
		while(esta_definida_actual(iter)){
			printf("%s ",actual_iter(iter));
			avanzar_iter(iter);
		}
		printf("\n");

		//liberacion memoria
		liberar_iterador(iter);

	}

}
void imprimir_por_niveles(binario_t b){
	if(!es_vacio_binario(b)){
		
		int contador= 0;
		cola_binarios_t c = crear_cola_binarios();
		if(!es_vacio_binario(izquierdo(b))){
			encolar(izquierdo(b),c);	
			contador++;
		}
		if(!es_vacio_binario(derecho(b))){
			encolar(derecho(b),c);	
			contador++;
		}
		imprimir_por_niveles_aux(c,contador);
		liberar_cola_binarios(c);
		printf("%s ", frase_info(raiz(b)) );
	}
	

}

bool pertenece(int i, cadena_t cad){
	localizador_t loc = inicio_cadena(cad);
	loc = siguiente_clave(i, loc, cad);
	if (es_localizador(loc)){
		return true;
	}else return false;
	

}

nat longitud(cadena_t cad){
		nat longitud = 0;
	if (!es_vacia_cadena(cad)) {
	localizador_t loc = inicio_cadena(cad);
		longitud++;
		while (!es_final_cadena(loc, cad)) {
			longitud++;
			loc = siguiente(loc, cad);
		}
	}
	return longitud;
}

bool son_iguales(cadena_t c1, cadena_t c2){
	localizador_t loc1 = inicio_cadena(c1);
	localizador_t loc2 = inicio_cadena(c2);
	bool iguales = true;
	while (es_localizador(loc1) && es_localizador(loc2)) {
		if (numero_info (info_cadena(loc1, c1)) != numero_info( info_cadena(loc2, c2))) {
			iguales = false;
		}
		else if (strcmp(frase_info(info_cadena(loc1, c1)), frase_info(info_cadena(loc2, c2))) != 0){
			iguales = false;
		}
		loc1 = siguiente(loc1, c1);
		loc2 = siguiente(loc2, c2);
	}
	if ((es_localizador(loc1) && !es_localizador(loc2)) || (!es_localizador(loc1) && es_localizador(loc2))) {
		iguales = false;
	}

	return iguales;
}

cadena_t concatenar(cadena_t c1, cadena_t c2){
  cadena_t res;
  localizador_t loc1,loc2;
  loc1 = inicio_cadena(c1);
  loc2 = final_cadena(c1);

  res = segmento_cadena(loc1,loc2,c1);
  
  loc1 = inicio_cadena(c2);
  
  while(es_localizador(loc1)){
  	insertar_al_final(copia_info(info_cadena(loc1,c2)),res);
  	loc1 = siguiente(loc1,c2);
  }

  return res;

}

void ordenar(cadena_t &cad){
	if (!es_vacia_cadena(cad)) {
		localizador_t loc1 = inicio_cadena(cad);
		localizador_t loc2;

		while (es_localizador(loc1)) {
			loc2 = siguiente(loc1, cad);
			while (es_localizador(loc2)) {
				if (numero_info(info_cadena(loc1, cad)) > numero_info(info_cadena(loc2, cad))) {
					//utilizamos intercambiar para ordenar la cadena sin cambiar de lugar los nodos
					intercambiar(loc1, loc2, cad);
				}
				loc2 = siguiente(loc2, cad);
			}
			loc1 = siguiente(loc1, cad);

			
		}

	}
}

void cambiar_todos(int original, int nuevo, cadena_t &cad){
	localizador_t loc = inicio_cadena(cad);
	if (!es_vacia_cadena(cad)) {
		while (es_localizador(loc)) {
			//si encontramos en numero original
			if (numero_info(info_cadena(loc,cad)) == original) {
				//necesitamos generar un nuevo arrelgo con la frase que contenia el nodo original
				//luego generamos un nuevo nodo, copiamos el dato numerico y enlasamos el nuevo arreglo
				//de esta forma no estamos utilisando el mismo arreglo de char, esto es un problema cuando
				//invocamos a liberar_info en el nodo con el dato numerico original
				char *nueva_frase = new char[strlen(frase_info(info_cadena(loc,cad)))+1];
				strcpy(nueva_frase,frase_info(info_cadena(loc,cad)));
				info_t dato = crear_info(nuevo,nueva_frase);
				info_t borrar = info_cadena(loc,cad);
				cambiar_en_cadena(dato,loc,cad);
				liberar_info(borrar);
			}
			
			loc = siguiente(loc,cad);
		}
	}
}

cadena_t subcadena(int menor, int mayor, cadena_t cad){
	//inicio el localizador al comienso de cad
	localizador_t loc = inicio_cadena(cad);
	// creo una nueva cadena res
	cadena_t res = crear_cadena();
	while (es_localizador(loc) && (numero_info(info_cadena(loc, cad)) < menor)) {
		loc = siguiente(loc, cad);
	}
	while (es_localizador(loc) && (numero_info(info_cadena(loc, cad)) <= mayor)) {
		info_t copia = copia_info(info_cadena(loc,cad));
		insertar_al_final(copia, res);
		loc = siguiente(loc,cad);
	}

	return res;
}
