#include <stdio.h>
#include <stdlib.h>

TColaCP crear cola cp(int (*f)(TEntrada, TEntrada)){
    TColaCP cola = NULL;

    return cola;
}
private int f(TEntrada arg1, TEntrada arg2){
    if(arg1.clave>)
    return -1;
}

typedef struct cola_con_prioridad {
unsigned int cantidad_elementos;
TNodo raiz;
} * TColaCP;

typedef struct nodo {
TEntrada entrada;
struct nodo * padre;
struct nodo * hijo_izquierdo;
struct nodo * hijo_derecho;
} * TNodo;

typedef struct entrada {
TClave clave;
TValor valor;
} * TEntrada;

typedef void * TClave;
typedef void * TValor;




int main()
{
    printf("Hello world!\n");
    return 0;
}
