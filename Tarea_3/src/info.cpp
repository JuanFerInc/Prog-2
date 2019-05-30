/* 48191081 */
#include "../include/info.h"

#include <string.h>
#include <stdio.h>
#include <limits.h>

struct  rep_info {
	int num;
	//arreglo cuyo tamano sera determinado en tiempo de ejecucion
	char *fra;
};

typedef rep_info *info_t;

info_t crear_info(int num, char*frase){
	info_t myinfo = new rep_info;
	myinfo->num = num;
	// comparte memoria con el parametro frase
	myinfo->fra = frase;

	return myinfo;
}

info_t copia_info(info_t info){
	info_t myinfo = new rep_info;
	myinfo->num = info->num;
	//genera un arreglo del mismo tamano que contiene el parametro
	myinfo->fra = new char[strlen(info->fra)+1];
	//copio el contenido del arreglo recivido en el nuevo arreglo
	strcpy(myinfo->fra, info->fra);

	return myinfo;
}

void liberar_info(info_t &info){
	// retorna la memoria solicitada para el arreglo
	delete[] info->fra;
	// retorna la memoria solicitada por el puntero
	delete info;

}

int numero_info(info_t info){
	return info->num;
}

char *frase_info(info_t info){
	return info->fra;
}


bool son_iguales(info_t i1, info_t i2){
	// hace la comparacion mas rapida primero, 
	// si esta se cumple compara frase 1 con frase 2
	if ((i1->num == i2->num) && (strcmp(i1->fra, i2->fra) == 0)){
		return true;
	}else return false;
}

bool es_valida_info(info_t info){
	return (info->num != INT_MAX);
}

char* info_a_texto(info_t info){
	char copia_num[11];
	sprintf(copia_num, "%d",info->num);
	char *texto = new char[strlen(copia_num) + strlen(info->fra)+4];
	strcpy(texto,"(");
	strcat(texto,copia_num);
	strcat(texto, ",");
	strcat(texto, info->fra);
	strcat(texto, ")");

	return texto;

}

info_t leer_info(int max){
	info_t leido;
	int num;
	char *cadena = new char[max+1];
	char simbolo;
	bool error = false;
	scanf(" %c", &simbolo);
	if (simbolo != '('){
		error = true;
	}
	else {
		scanf("%d", &num);
		scanf("%c", &simbolo);
		if (simbolo != ','){
			error = true;
		}
		else {
			int pos = 0;
			int c = getchar();
			while ((c != ')') && (c != '\n')){
				cadena[pos] = c;
				pos++;
				c = getchar();
			}
			cadena[pos]= '\0';
			if (c == '\n'){
				error = true;
				ungetc('\n', stdin);
			}
		}
	}
	if(error){
		char *nulo = new char[1];
		nulo[0] = '\0';
		leido = crear_info(INT_MAX, nulo);
	}else{
		char *frase = new char[strlen(cadena)+1];
		strcpy(frase,cadena);
		leido = crear_info(num,frase);
	}
	delete[] cadena;
	return leido;
}
info_t combinar_info(info_t i1, info_t i2){
	//guarda la suma de los numero
	int num = i1->num + i2->num;
	info_t myinfo = new rep_info;
	myinfo->num = num;
	//genera un arreglo con el largo de las dos frases
	myinfo->fra = new char[strlen(i1->fra) + strlen(i2->fra)+1];
	//inicio un arreglo auxiliar para guardar las fraces concatenadas
	char *copia_frase =  new char[strlen(i1->fra) + strlen(i2->fra)+1];
	//copio las frase del arreglo auxiliar para evitar compartir memoria
	strcpy(copia_frase, i1->fra);
	strcat(copia_frase, i2->fra);
	strcpy(myinfo->fra, copia_frase);

	delete[] copia_frase;
	return myinfo;

}
