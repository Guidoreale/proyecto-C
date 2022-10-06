#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FALSE 0;
#define TRUE 1;
#define CCP_NO_INI 2;
#define POS_NULA NULL;
#define ELE_NULO NULL;

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


TNodo crear_nodo(TEntrada aInsertar, TNodo padre);

void reacomodar(struct nodo *pNodo, TColaCP pPrioridad);

TColaCP crearCola(int (*f)(TEntrada, TEntrada)){
    TColaCP nuevaCola = (TColaCP) malloc(sizeof(struct cola_con_prioridad));
    nuevaCola ->cantidad_elementos = 0;
    nuevaCola ->raiz = NULL;
    nuevaCola ->comparador = (*f);
    return nuevaCola;
}


int f(TEntrada a, TEntrada b){
    int valor = 0;
    if(a->clave > b->clave)
       valor = -1;
    else if (b->clave > a->clave)
       valor = 1;
    else
        valor = 0;
    return valor;
    }

    int cp_cantidad(TColaCP cola){
        return cola->cantidad_elementos;
    }

   int logaritmo(int numero){
      int ret = log(numero)/log(2);
//      printf("%d \n", ret);
      return ret;
   }


   TEntrada cp_eliminar(TColaCP cola){
        TEntrada entrada;
        if (cola == NULL){
            entrada = ELE_NULO;
        }
        else if (cola->cantidad_elementos==1) {
            entrada = cola->raiz->entrada;
            cola->raiz->entrada = ELE_NULO;
            cola->raiz = POS_NULA;
        }
        else if (cola->cantidad_elementos == pow (2,(logaritmo(cola->cantidad_elementos) + 1)) - 1){
            reacomodar(cola->raiz->hijo_izquierdo, cola);
            }


        return entrada;
   }

void reacomodar(TNodo nodo , TColaCP pPrioridad) {

}

int insertarPerfecto(TEntrada aInsertar,TColaCP cola){
        TNodo temp; //TNodo ya es un puntero, por lo tento no deberiamos usar *temp sino temp.
        temp = cola->raiz;
        while(temp->hijo_izquierdo != NULL){
            temp = temp->hijo_izquierdo;
        }
        TNodo nuevo = (TNodo) malloc(sizeof(struct nodo));
        nuevo->entrada = aInsertar;
        temp->hijo_izquierdo = nuevo;
        nuevo->padre = temp;
        cola->cantidad_elementos++;
        nuevo->hijo_derecho = NULL;
        nuevo->hijo_izquierdo = NULL;
        return 1;
}

    int buscar(int nivel, TEntrada aInsertar, TNodo nodo , TColaCP cola){
        printf("estoy dentro de buscar y la cantidad de elementos actuales de la cola es: %d \n", cola->cantidad_elementos);
        int nivel_actual = nivel; // empieza en 1, la vuelta del derecho entra con nivel_actual = 2
        int retorno = 0;
        if (nivel_actual < logaritmo(cola->cantidad_elementos)){
            nivel_actual++;
            retorno = buscar(nivel_actual, aInsertar, nodo->hijo_izquierdo, cola);
            if (!retorno)
                retorno = buscar(nivel_actual, aInsertar, nodo->hijo_derecho, cola);
        }
        else {
            if (nodo->hijo_izquierdo == NULL) {
                TNodo nuevo = crear_nodo(aInsertar, nodo);
                cola->cantidad_elementos++;
                nodo->hijo_izquierdo = nuevo;
                retorno = 1;
            } else if (nodo->hijo_derecho == NULL) {
                TNodo nuevo = crear_nodo(aInsertar, nodo);
                nodo->hijo_derecho = nuevo;
                cola->cantidad_elementos++;
                retorno = 1;
            }
        }
    return retorno;
}

TNodo crear_nodo(TEntrada aInsertar, TNodo padre) {
    TNodo nuevo = (TNodo) malloc(sizeof(struct nodo));
    nuevo->entrada = aInsertar;
    nuevo->hijo_derecho = POS_NULA;
    nuevo->hijo_izquierdo = POS_NULA;
}

int insertar(TColaCP cola, TEntrada aInsertar){
    int aRetornar = 0;
    if (cola-> cantidad_elementos == 0){
        TNodo nuevo = (TNodo) malloc(sizeof(struct nodo));
        nuevo->entrada = aInsertar;
        cola ->raiz = nuevo;
        cola->raiz->padre = POS_NULA;
        cola ->cantidad_elementos++;
        aRetornar = 1;
        nuevo->hijo_derecho = POS_NULA;
        nuevo->hijo_izquierdo = POS_NULA;
    }
    else
        if (cola->cantidad_elementos == pow (2,(logaritmo(cola->cantidad_elementos) + 1)) - 1){
                insertarPerfecto(aInsertar,cola);
        }
            else
                aRetornar = buscar(1,aInsertar, cola->raiz,cola);

    return aRetornar;
}

int printearPreorden(TColaCP cola, TNodo raiz){
        printf("valor = %d \n", (raiz->entrada)->valor);
        if (raiz->hijo_izquierdo != NULL)
            printearPreorden(cola, raiz->hijo_izquierdo);
        if (raiz->hijo_derecho != NULL)
            printearPreorden(cola, raiz->hijo_derecho);
    return 1;
    }
int nada(TEntrada e1, TEntrada e2){
    return 0;
}

void miniTester(){
        TColaCP  cola = crearCola(nada);
        for(int i = 0; i < 10; i++){
            TEntrada ent = (TEntrada)malloc(sizeof(struct entrada));
            insertar(cola, ent);
            asm("NOP");
        }

    }

int main()
{
    miniTester();


    printf("si logaritmo() anda correctamente esto deberia mostrar 2: %d \n", logaritmo(4));
    printf("si logaritmo() anda correctamente esto deberia mostrar 2: %d \n", logaritmo(6));
    printf("si logaritmo() anda correctamente esto deberia mostrar 3: %d \n", logaritmo(9));
    printf("si logaritmo() anda correctamente esto deberia mostrar 4: %d \n", logaritmo(17));
    printf("si logaritmo() anda correctamente esto deberia mostrar 5: %d \n", logaritmo(33));
    printf("\n logcasero no es el problema, por lo tanto el problema esta en el incremento de \n la cantidad de elementos cuando se realiza la insercion.\n \n estos 0..1 que siguen corresponden a la consulta de logaritmo cada vez que se quiere realizar la insercion.\n por lo tanto no se esta aumentando la cantidad de elementos de la cola \n");
    printf("\n inserta solo 2 nodos, el primero y el segundo(casos base), luego no entra al algoritmo buscar correctamente \n y deja de insertar\n");

    TColaCP cola = crearCola(f);
    TEntrada arreglo[8];
    TEntrada entr1 = (TEntrada) malloc(sizeof(struct entrada));
    TClave c1 = (TClave) 1;
    TValor v1 = (TValor) 2;
    entr1->clave = c1;
    entr1->valor = v1;
    arreglo[0] = entr1;

    TEntrada entr2 = (TEntrada) malloc(sizeof(struct entrada));
    TClave c2 = (TClave) 3;
    TValor v2 = (TValor) 4;
    entr2->clave = c2;
    entr2->valor = v2;
    arreglo[1] = entr2;

    TEntrada entr3 = (TEntrada) malloc(sizeof(struct entrada));
    TClave c3 = (TClave) 5;
    TValor v3 = (TValor) 6;
    entr3->clave = c3;
    entr3->valor = v3;
    arreglo[2] = entr3;

    TEntrada entr4 = (TEntrada) malloc(sizeof(struct entrada));
    TClave c4 = (TClave) 7;
    TValor v4 = (TValor) 8;
    entr4->clave = c4;
    entr4->valor = v4;
    arreglo[3] = entr4;

    TEntrada entr5 = (TEntrada) malloc(sizeof(struct entrada));
    TClave c5 = (TClave) 9;
    TValor v5 = (TValor) 10;
    entr5->clave = c5;
    entr5->valor = v5;
    arreglo[4] = entr5;

    TEntrada entr6 = (TEntrada) malloc(sizeof(struct entrada));
    TClave c6 = (TClave) 11;
    TValor v6 = (TValor) 12;
    entr6->clave = c6;
    entr6->valor = v6;
    arreglo[5] = entr6;

    TEntrada entr7 = (TEntrada) malloc(sizeof(struct entrada));
    TClave c7 = (TClave) 13;
    TValor v7 = (TValor) 14;
    entr7->clave = c7;
    entr7->valor = v7;
    arreglo[6] = entr7;

    TEntrada entr8 = (TEntrada) malloc(sizeof(struct entrada));
    TClave c8 = (TClave) 15;
    TValor v8 = (TValor) 16;
    entr8->clave = c8;
    entr8->valor = v8;
    arreglo[7] = entr8;

    for(int i = 0; i < 8; i++){
        insertar(cola, arreglo[i]);
    }
   printearPreorden(cola, cola->raiz);
    return 0;
}
