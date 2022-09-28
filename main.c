#include <stdio.h>
#include <stdlib.h>

typedef void * TClave;
typedef void * TValor;

typedef struct entrada {
    TClave clave;
    TValor valor;
} * TEntrada;

typedef struct nodo {
    TEntrada entrada;
    struct nodo * padre;
    struct nodo * hijo_izquierdo;
    struct nodo * hijo_derecho;
} * TNodo;

typedef struct cola_con_prioridad {
    unsigned int cantidad_elementos;
    TNodo raiz;
    int (*comparador)(TEntrada, TEntrada);
} * TColaCP;

TColaCP crearCola(int (*f)(TEntrada, TEntrada)){
    TColaCP nuevaCola = (TColaCP) malloc(sizeof(struct cola_con_prioridad));
    nuevaCola ->cantidad_elementos = 0;
    nuevaCola ->raiz = NULL;
    nuevaCola ->comparador = (*f);
    return nuevaCola;
}

 void intercambio(TNodo a, TNodo b){
            int temp = a->entrada;
            a->entrada = b->entrada;
            b->entrada = temp;
        }



int f(TEntrada a, TEntrada b){
    int valor = 0;
    if(a->clave > b->clave)
       valor = -1;
    else if (b->clave > a->clave)
       valor = 1;
    else
        valor = 0;
    }




int main()
{
    printf("Hello world!\n");
    return 0;
}
