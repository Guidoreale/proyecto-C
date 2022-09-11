#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 2000

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



void f(char *file1){

    char buffer[SIZE];
    FILE *archivo1 = fopen(file1,"r");
    int encontre = 0;
    char ch;
    char cadena[SIZE];
    int i = 0;
    int entero = 0;
    if(!archivo1){
        printf("Error al abrir el archivo\n");
    }

    else{

        while(!encontre &&(ch = fgetc(archivo1))!= EOF){

            if(ch != ';' ){
                cadena[i]= ch;
                i++;
                }
            if (ch==';')
                encontre = 1;
        }
        sscanf(cadena, "%d", &entero);
        printf("%i",entero);
        if (!encontre)
            printf("NO encontramos %s",buffer);

        fclose(archivo1);
    }

    }



int main(int argc,char *args[])
{
    if (argc==2){
        f(args[1]);
    }
    else{
        printf("error en args\n");
    }

    return 0;
}
