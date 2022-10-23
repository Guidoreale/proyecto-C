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
    unsigned int cantidad_elementos;
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
    if (a->valor < b->valor)
        return -1;
    else if(a->valor > b->valor)
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
        int nivel_actual = nivel; // empieza en 1, la vuelta del derecho entra con nivel_actual = 2
        int retorno = 0;
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
}

int insertar(TColaCP cola, TEntrada aInsertar){
    int aRetornar = 0;
    if (cola-> cantidad_elementos == 0){
        TNodo nuevo = (TNodo) malloc(sizeof(struct nodo));
        nuevo->entrada = aInsertar;
        cola ->raiz = nuevo;
        cola->raiz->padre = POS_NULA;
        cola ->cantidad_elementos++;
        aRetornar = TRUE;
        nuevo->hijo_derecho = POS_NULA;
        nuevo->hijo_izquierdo = POS_NULA;
    }
    else
        if (cola->cantidad_elementos == pow (2,(logaritmo(cola->cantidad_elementos) + 1)) - 1){
                insertarPerfecto(aInsertar,cola);
        }
            else
                aRetornar = buscar(1,aInsertar, cola->raiz,cola);

    //reacomodarInsercion();
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
    for(int i = 0; i <9; i++){
        TEntrada entr = (TEntrada) malloc(sizeof(struct entrada));
        int* c = (int*)malloc(sizeof (int));
        int* v = (int*)malloc(sizeof (int));
        *c = i;
        *v = i+100;
        entr->clave = c;
        entr->valor = v;
        cp_insertar(cola, entr);
    }
    //cp_eliminar(cola);
     cp_destruir(cola,fEliminar);
    printearPreorden(cola, cola->raiz);
    return 0;
}
