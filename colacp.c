#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FALSE 0
#define TRUE 1
#define CCP_NO_INI 2
#define POS_NULA NULL
#define ELE_NULO NULL

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
    int cantidad_elementos;
    TNodo raiz;
    int (*comparador)(TEntrada, TEntrada);
} * TColaCP;


TNodo crear_nodo(TEntrada aInsertar, TNodo padre);



TColaCP crearCola(int (*f)(TEntrada, TEntrada)){
    TColaCP nuevaCola = (TColaCP) malloc(sizeof(struct cola_con_prioridad));
    nuevaCola ->cantidad_elementos = 0;
    nuevaCola ->raiz = NULL;
    nuevaCola ->comparador = (*f);
    return nuevaCola;
}


int f(TEntrada a, TEntrada b){
    if (*((int*)a->clave) < *((int*)b->clave))
        return -1;
    else if(*((int*)a->clave) > *((int*)b->clave))
        return 1;
    else
        return 0;
    }

int cp_cantidad(TColaCP cola){
    return cola->cantidad_elementos;
}
void fEliminar(TEntrada entrada){
    free(entrada->clave);
    free(entrada->valor);
    free(entrada);
}
   int logaritmo(int numero){
      int ret = log(numero)/log(2);
      return ret;
   }

    TNodo eliminarPerfecto(TColaCP cola){
    TNodo iterador = cola->raiz;
        while (iterador->hijo_derecho != NULL)
            iterador = iterador->hijo_derecho;
        return iterador;
    }
    void reacomodar(TNodo nodo, TColaCP cola){
        TEntrada temp = NULL;
        if (nodo->hijo_izquierdo != NULL) {
            if (nodo->hijo_derecho == NULL || cola->comparador(nodo->hijo_izquierdo->entrada, nodo->hijo_derecho->entrada) == 0 || cola->comparador(nodo->hijo_izquierdo->entrada, nodo->hijo_derecho->entrada) == -1) {
                temp = nodo->hijo_izquierdo->entrada;
                nodo->hijo_izquierdo->entrada = nodo->entrada;
                nodo->entrada = temp;
                reacomodar(nodo->hijo_izquierdo, cola);
            }
            else {
                temp = nodo->hijo_derecho->entrada;
                nodo->hijo_derecho->entrada = nodo->entrada;
                nodo->entrada = temp;
                reacomodar(nodo->hijo_derecho, cola);
            }
        }
    }

    TNodo buscarEliminable(TNodo nodo, int nivel, int nivelMaximo,TNodo actual){
        TNodo aRetornar = actual;
        if (nivel == nivelMaximo)
            aRetornar = nodo;
        if (nodo->hijo_izquierdo != NULL)
            aRetornar = buscarEliminable(nodo->hijo_izquierdo,nivel + 1, nivelMaximo,aRetornar);
        if (nodo->hijo_derecho != NULL)
            aRetornar = buscarEliminable(nodo->hijo_derecho, nivel + 1, nivelMaximo,aRetornar);
        return aRetornar;
    }
    TEntrada cp_eliminar(TColaCP cola){
            TEntrada aRetornar  = cola->raiz->entrada;
            TNodo aEliminar = NULL;
            if (cola == NULL){
                exit(CCP_NO_INI);
            }
            else{
                if(cola->cantidad_elementos==0){
                    return ELE_NULO;
                }
                else if(cola->cantidad_elementos==1){
                    aEliminar = cola->raiz;
                    cola->raiz = NULL;
                }
                else {
                    if (cola->cantidad_elementos == pow(2, (logaritmo(cola->cantidad_elementos) + 1)) - 1) {
                        aEliminar = eliminarPerfecto(cola);
                        aEliminar->padre->hijo_derecho = NULL;
                    }
                    else {
                        aEliminar = buscarEliminable(cola->raiz, 0, (logaritmo(cola->cantidad_elementos)), cola->raiz);
                        if (aEliminar->padre->hijo_izquierdo == aEliminar){
                            aEliminar->padre->hijo_izquierdo = NULL;
                        } else{
                            aEliminar->padre->hijo_derecho = NULL;
                        }
                    }
                    cola->raiz->entrada = aEliminar->entrada;
                }
            }
            free(aEliminar);
            reacomodar(cola->raiz, cola);
            return aRetornar;
    }


    void destruirRecursivo(TNodo nodo, void (*fEliminar)(TEntrada)){
        if(nodo->hijo_izquierdo!=NULL)
            destruirRecursivo(nodo->hijo_izquierdo,fEliminar);

        if(nodo->hijo_derecho!=NULL)
            destruirRecursivo(nodo->hijo_derecho,fEliminar);
        fEliminar(nodo->entrada);
        free(nodo);
    }

    void cp_destruir(TColaCP cola, void (*fEliminar)(TEntrada) ){
        if (cola == NULL)
            exit(CCP_NO_INI);
        else
            destruirRecursivo(cola->raiz,fEliminar);
    }
    void reacomodarInsercion(TNodo nodo) {
        TEntrada aux;
        if ((nodo->padre!=NULL) && (f(nodo->padre->entrada,nodo->entrada)==1)) {
            aux = nodo->entrada;
            nodo->entrada = nodo->padre->entrada;
            nodo->padre->entrada = aux;
            reacomodarInsercion(nodo);
        }
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
        reacomodarInsercion(nuevo);
        return TRUE;
    }

    int buscar(int nivel, TEntrada aInsertar, TNodo nodo , TColaCP cola){
        int nivel_actual = nivel; // empieza en 1, la vuelta del derecho entra con nivel_actual = 2
        int retorno = FALSE;
        if (nivel_actual < logaritmo(cola->cantidad_elementos)){ //logaritmo en base 2 de los elementos
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
                retorno = TRUE;

            } else if (nodo->hijo_derecho == NULL) {
                TNodo nuevo = crear_nodo(aInsertar, nodo);
                nodo->hijo_derecho = nuevo;
                cola->cantidad_elementos++;
                retorno = TRUE;
            }
        }
    return retorno;
}

TNodo crear_nodo(TEntrada aInsertar, TNodo padre) {
    TNodo nuevo = (TNodo) malloc(sizeof(struct nodo));
    nuevo->entrada = aInsertar;
    nuevo->hijo_derecho = POS_NULA;
    nuevo->hijo_izquierdo = POS_NULA;
    nuevo->padre = padre;
    reacomodarInsercion(nuevo);
    return nuevo;
}

int cp_insertar(TColaCP cola, TEntrada aInsertar){
    int aRetornar = FALSE;
    if (cola-> cantidad_elementos == 0){
        TNodo nuevo = (TNodo) malloc(sizeof(struct nodo));
        nuevo->entrada = aInsertar;
        cola ->raiz = nuevo;
        cola->raiz->padre = POS_NULA;
        cola ->cantidad_elementos++;
        nuevo->hijo_derecho = POS_NULA;
        nuevo->hijo_izquierdo = POS_NULA;
        aRetornar = TRUE;
    }
    else
        if (cola->cantidad_elementos == pow (2,(logaritmo(cola->cantidad_elementos) + 1)) - 1){
               aRetornar= insertarPerfecto(aInsertar,cola);
        }
        else
            aRetornar = buscar(1,aInsertar, cola->raiz,cola);


    return aRetornar;
}



int printearPreorden(TColaCP cola, TNodo raiz){
        printf("clave = %d \n", *((int*)raiz->entrada->clave));
        if (raiz->hijo_izquierdo != NULL)
            printearPreorden(cola, raiz->hijo_izquierdo);
        if (raiz->hijo_derecho != NULL)
            printearPreorden(cola, raiz->hijo_derecho);
    return TRUE;
    }



int main()
{

    TColaCP cola = crearCola(f);

    TEntrada arreglo[8];

    TEntrada entr1 = (TEntrada) malloc(sizeof(struct entrada));
    int* c1 = (int*)malloc(sizeof (int));
    int* v1 = (int*)malloc(sizeof (int));
    *c1 = 7;
    *v1 = 5;
    entr1->clave = c1;
    entr1->valor = v1;
    arreglo[0] = entr1;

    TEntrada entr2 = (TEntrada) malloc(sizeof(struct entrada));
    int* c2 = (int*)malloc(sizeof (int));
    int* v2 = (int*)malloc(sizeof (int));
    *c2 = 1;
    *v2 = 5;
    entr2->clave = c2;
    entr2->valor = v2;
    arreglo[1] = entr2;

    TEntrada entr3 = (TEntrada) malloc(sizeof(struct entrada));
    int* c3 = (int*)malloc(sizeof (int));
    int* v3 = (int*)malloc(sizeof (int));
    *c3 = 9;
    *v3 = 5;
    entr3->clave = c3;
    entr3->valor = v3;
    arreglo[2] = entr3;

    TEntrada entr4 = (TEntrada) malloc(sizeof(struct entrada));
    int* c4 = (int*)malloc(sizeof (int));
    int* v4 = (int*)malloc(sizeof (int));
    *c4 = 6;
    *v4 = 5;
    entr4->clave = c4;
    entr4->valor = v4;
    arreglo[3] = entr4;


    TEntrada entr5 = (TEntrada) malloc(sizeof(struct entrada));
    int* c5 = (int*)malloc(sizeof (int));
    int* v5 = (int*)malloc(sizeof (int));
    *c5 = 3;
    *v5 = 5;
    entr5->clave = c5;
    entr5->valor = v5;
    arreglo[4] = entr5;

    TEntrada entr6 = (TEntrada) malloc(sizeof(struct entrada));
    int* c6 = (int*)malloc(sizeof (int));
    int* v6 = (int*)malloc(sizeof (int));
    *c6 = 2;
    *v6 = 5;
    entr6->clave = c6;
    entr6->valor = v6;
    arreglo[5] = entr6;

    TEntrada entr7 = (TEntrada) malloc(sizeof(struct entrada));
    int* c7 = (int*)malloc(sizeof (int));
    int* v7 = (int*)malloc(sizeof (int));
    *c7 = 8;
    *v7 = 5;
    entr7->clave = c7;
    entr7->valor = v7;
    arreglo[6] = entr7;

    TEntrada entr8 = (TEntrada) malloc(sizeof(struct entrada));
    int* c8 = (int*)malloc(sizeof (int));
    int* v8 = (int*)malloc(sizeof (int));
    *c8 = 5;
    *v8 = 5;
    entr8->clave = c8;
    entr8->valor = v8;
    arreglo[7] = entr8;

    for(int i = 0; i < 8; i++){
        cp_insertar(cola, arreglo[i]);
    }

    //cp_eliminar(cola);
     //cp_destruir(cola,fEliminar);
    printearPreorden(cola, cola->raiz);
    return 0;
}
