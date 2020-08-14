/*
Ejercicio 10c listas
Programa que inserta ordenado en lista doble
*/

#include <stdio.h>
#include <stdlib.h> //para malloc y system

typedef struct nodo_d {
	float dato; //Se decidio arbitrariamente usar float para el dato
	struct nodo_d *ant, *sig;
} nodo_d;

//La lista doble esta representada por una estructura con 2 punteros: primero y ultimo
typedef struct ldoble {
	nodo_d *prim, *ult;
} lista;

lista ins_ord(float dat, lista l) {
	nodo_d *nuevo;
	nodo_d *p;

	nuevo = (nodo_d*) malloc(sizeof(nodo_d));
	nuevo->dato = dat;
	if (l.prim == NULL) { //Lista vacia
		nuevo->ant = NULL;
		nuevo->sig = NULL;
		l.prim = nuevo;
		l.ult = nuevo;
	}
	else {
		if (dat < l.prim->dato){ //Primer elemento
			nuevo->sig = l.prim;
			nuevo->ant = NULL;
			l.prim->ant = nuevo;
			l.prim = nuevo;
		}
		else {
			p = l.prim;
			while (p->sig != NULL && p->sig->dato < dat) {
				p = p->sig;
			}
			nuevo->sig = p->sig;
			nuevo->ant = p;
			p->sig = nuevo;
			if (nuevo->sig != NULL) //Elemento intermedio
				nuevo->sig->ant = nuevo;
			else //Ultimo elemento
				l.ult = nuevo;
		}
	}
	return l;
}

//Destruye la lista liberando la memoria ocupada por cada nodo
lista destruir (lista l) {
	nodo_d *p, *aux;

	p = l.prim;
	while (p != NULL) {
		aux = p;
		p = p->sig;
		free (aux);
	}
	printf ("Lista destruida\n");
	l.prim = NULL;
	l.ult = NULL;
	return l;
}

int main() {
	nodo_d *p;
	float f;
	lista l;

	l.prim = NULL;
	l.ult = NULL;

	printf("Ingrese: dato (0 para terminar)\n");
	scanf("%f", &f);
	while (f > 0) {
		l = ins_ord(f, l);
		scanf("%f", &f);
	}

	printf("Muestro en orden ingresado\n");
	for (p = l.prim; p != NULL; p = p->sig)
		printf("Dato = %6.2f\n", p->dato);

	l = destruir (l);
	//system("pause");
	return 0;
}
