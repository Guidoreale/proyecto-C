#include <stdio.h>
#include <stdlib.h>

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

   int logCasero(int numero){
      int ret = (int)log(numero);
      return ret;
   }

    void reacomodarI(TNodo nodo, TColaCP cola){
    
    }

   TEntrada cp_eliminar(TColaCP cola){
        TEntrada entrada;
        if (cola == NULL){
            entrada = ELE_NULO;
        }
        else{
            entrada = cola->raiz->entrada;
            if (cola->raiz->hijo_derecho->entrada->clave > cola->raiz->hijo_izquierdo->entrada->clave){
                cola->raiz = NULL;
                reacomodar(cola->raiz->hijo_izquierdo, cola);
            }
        }

        return entrada;
   }

    int insertarPerfecto(TEntrada aInsertar,TColaCP cola){
        TNodo *temp;
        temp = &cola->raiz;
        while((*temp)->hijo_izquierdo != NULL){
            temp = (*temp)->hijo_izquierdo;
        }
        TNodo nuevo = (TNodo) malloc(sizeof(struct nodo));
        nuevo->entrada = aInsertar;
        (*temp)->hijo_izquierdo = nuevo;
        nuevo->padre = (*temp);
        cola->cantidad_elementos++;
        //¿es correcto el usp del puntero, tendriamos que usar malloc y free?
    }

    int buscar(int contador, TEntrada aInsertar, TNodo nodo , TColaCP cola){
        int cont = contador; // empieza en 1, la vuelta del derecho entra con cont = 2
        int retorno = 0;
        if (cont < (logCasero(cola->cantidad_elementos)/logCasero(2)) -1){
            cont++;
            retorno = buscar(cont,aInsertar,nodo->hijo_izquierdo,cola);
            if (!retorno)
                buscar(cont,aInsertar, nodo->hijo_derecho, cola);
        }
        else
            if (nodo->hijo_izquierdo == NULL){
                TNodo nuevo = (TNodo) malloc(sizeof(struct nodo));
                nuevo->entrada = aInsertar;
                nodo->hijo_izquierdo = nuevo;
                retorno = 1;
            }
            else
                if (nodo->hijo_derecho == NULL){
                    TNodo nuevo = (TNodo) malloc(sizeof(struct nodo));
                    nuevo->entrada = aInsertar;
                    nodo->hijo_derecho = nuevo;
                    retorno = 1;
                }       
    return retorno;
}

int insertar(TColaCP cola, TEntrada aInsertar){
    int aRetornar = 0;
    if (cola-> cantidad_elementos == 0){
        TNodo nuevo = (TNodo) malloc(sizeof(struct nodo));
        nuevo->entrada = aInsertar;
        cola ->raiz = nuevo;
        nuevo->padre = NULL;
        cola ->cantidad_elementos++;
        aRetornar = 1;
    }
    else
        if (cola->cantidad_elementos == 2^((logCasero(cola->cantidad_elementos)/logCasero(2) + 1)) -1){
                insertarPerfecto(aInsertar,cola);
        }
        else 
            if ((logCasero(cola->cantidad_elementos)/logCasero(2)) == 0){
                TNodo nuevo = (TNodo) malloc(sizeof(struct nodo));
                nuevo->entrada = aInsertar;
                cola ->raiz->hijo_izquierdo = nuevo;
                nuevo->padre = cola->raiz;
                aRetornar = 1;
            }
            else
                aRetornar = buscar(1,aInsertar, cola->raiz,cola);  

    return aRetornar;
}





int main()
{
    TColaCP cola = crearCola(f);    
    TEntrada entr;
    entr->clave = (TClave) 1;
    entr->valor = (TValor) 3;
    insertar(cola, entr);



   for(int i=0; i<4; i++){
        printf("cola %i: \n", cola->raiz);
        }

    return 0;
}
