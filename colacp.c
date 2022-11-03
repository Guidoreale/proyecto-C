#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "colacp.h"



TNodo crear_nodo(TEntrada aInsertar, TNodo padre) {
    TNodo nuevo = (TNodo) malloc(sizeof(struct nodo));//Reservamos memoria del nodo correspondiente a crear
    if(nuevo!=NULL) {
        nuevo->entrada = aInsertar;
        nuevo->hijo_derecho = POS_NULA;
        nuevo->hijo_izquierdo = POS_NULA;
        nuevo->padre = padre;
        // devolvemos el nuevo nodo creado, con sus respectivas referencias al padre y sus hijos, y añadimos una entrada
    }
    return nuevo; // si el malloc falla, devolvera nulo.
}

TColaCP crear_cola_cp(int (*f)(TEntrada, TEntrada)){
    TColaCP nuevaCola = (TColaCP) malloc(sizeof(struct cola_con_prioridad));
    if(nuevaCola!=NULL){
        nuevaCola ->cantidad_elementos = 0;
        nuevaCola ->raiz = NULL;
        nuevaCola ->comparador = (*f);
    }
        return nuevaCola;
}


int ordenAscendente(TEntrada a, TEntrada b){
    if (*((float*)a->clave) < *((float*)b->clave))
        return -1;
    else if(*((float*)a->clave) > *((float*)b->clave))
        return 1;
    else
        return 0;
    }

int ordenDescendente(TEntrada a, TEntrada b){
    if (*((float*)a->clave) > *((float*)b->clave))
        return -1;
    else if(*((float*)a->clave) < *((float*)b->clave))
        return 1;
    else
        return 0;
    }

int cp_cantidad(TColaCP cola){
    return cola->cantidad_elementos;
}

void fEliminar(TEntrada entrada){//Liberamos memoria de las claves, valores y luego de sus entradas
    free(entrada->clave);
    free(entrada->valor);
    free(entrada);
}

int logaritmo(int numero){ //computa al logaritmo sin decimales (int)
    int ret = log(numero)/log(2);
    return ret;
}

TNodo BuscarEliminarPerfecto(TColaCP cola){ //En caso de que el arbol sea perfecto buscar el ultimo elemento insertado en el ultimo nodo a la derecha
    TNodo iterador = cola->raiz;
    while (iterador->hijo_derecho != NULL)
        iterador = iterador->hijo_derecho;
    return iterador;
}

void reacomodarPostEliminación(TNodo nodo, TColaCP cola){ //Reacomoda el arbol una vez eliminado un nodo
    TEntrada temp = NULL;
    if (cola->cantidad_elementos == 0){
        exit(CCP_NO_INI);
    }
    if (nodo->hijo_izquierdo != NULL) {
        if ((nodo->hijo_derecho == NULL) || cola->comparador(nodo->hijo_izquierdo->entrada, nodo->hijo_derecho->entrada) == 0 || cola->comparador(nodo->hijo_izquierdo->entrada, nodo->hijo_derecho->entrada) == -1) {
            if(cola->comparador(nodo->hijo_izquierdo->entrada,nodo->entrada) == -1){
                temp = nodo->hijo_izquierdo->entrada;
                nodo->hijo_izquierdo->entrada = nodo->entrada;
                nodo->entrada = temp;
            }
            reacomodarPostEliminación(nodo->hijo_izquierdo, cola);
        }
        else {
            temp = nodo->hijo_derecho->entrada;
            nodo->hijo_derecho->entrada = nodo->entrada;
            nodo->entrada = temp;
            reacomodarPostEliminación(nodo->hijo_derecho, cola);
        }
    }
}

TNodo buscarEliminable(TNodo nodo, int nivel, int nivelMaximo,TNodo actual){ //En caso de que el arbol no sea perfecto, se utiliza este procedimiento
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
    if (cola == NULL){ //Caso cola nula
        exit(CCP_NO_INI);
    }
    else{
        if(cola->cantidad_elementos==0){ //Caso cola vacia
            return ELE_NULO;
        }
        else if(cola->cantidad_elementos==1){ //Caso cola con un solo elemento
            aEliminar = cola->raiz;
            cola->raiz = NULL;
        }
            else {
                if (cola->cantidad_elementos == pow(2, (logaritmo(cola->cantidad_elementos) + 1)) - 1) { //Caso en el que el arbol sea perfecto
                    aEliminar = BuscarEliminarPerfecto(cola);
                    aEliminar->padre->hijo_derecho = NULL;
            }
            else {
                aEliminar = buscarEliminable(cola->raiz, 0, (logaritmo(cola->cantidad_elementos)), cola->raiz);//Caso en el que el arbol NO sea perfecto
                if (aEliminar->padre->hijo_izquierdo == aEliminar){
                    aEliminar->padre->hijo_izquierdo = NULL;//Eliminamos referencias
                }
                else{
                    aEliminar->padre->hijo_derecho = NULL;
                }
            }
                cola->raiz->entrada = aEliminar->entrada;
            }
    }
    free(aEliminar);//Liberamos memoria del nodo
    cola->cantidad_elementos--;//Reducimos la cantidad de elementos del arbol luego de eliminar el nodo raiz
    if(cola!= NULL && cola->cantidad_elementos != 0)
        reacomodarPostEliminación(cola->raiz, cola); //Inicia el reacomodo post eliminacion
    return aRetornar;
    }


void destruirRecursivo(TNodo nodo, void (*fEliminar)(TEntrada)){ //Destruye recursivamente las entradas y el nodo en cuestion, en POSTORDEN(primero se visitan los hijos y por ultimo la raiz)  ya que es el recorrido idoneo para liberar la memoria de un arbol.
    if(nodo->hijo_izquierdo!=NULL)
        destruirRecursivo(nodo->hijo_izquierdo,fEliminar);

    if(nodo->hijo_derecho!=NULL)
        destruirRecursivo(nodo->hijo_derecho,fEliminar);
    fEliminar(nodo->entrada);//llamada a funcion fEliminar, elimina clave, valor y entrada del nodo
    free(nodo);
}

void cp_destruir(TColaCP cola, void (*fEliminar)(TEntrada) ){
    if (cola == NULL)//En caso de que la cola sea nula
        exit(CCP_NO_INI);

    if(cola->raiz != NULL)
        destruirRecursivo(cola->raiz, fEliminar);//Se liberara recursivamente a los nodos y sus respectivas entradas

    free(cola);//Por ultimo liberamos memoria de la cola

}

void reacomodarInsercion(TNodo nodo,TColaCP cola) {//Reacomoda al arbol POST insersion de un nodo
    TEntrada aux;
    if ((nodo->padre!=NULL) && (cola->comparador(nodo->padre->entrada,nodo->entrada)==1)) {
        aux = nodo->entrada;
        nodo->entrada = nodo->padre->entrada;
        nodo->padre->entrada = aux;//Swap de entradas, realizamos este swap para reordenar al arbol de forma simple sin perder o complejizar las referencias
        reacomodarInsercion(nodo->padre, cola);
    }
}

int insertarPerfecto(TEntrada aInsertar,TColaCP cola){ //Inserta nodo en caso de que el arbol sea perfecto
    TNodo temp;
    temp = cola->raiz;
    while(temp->hijo_izquierdo != NULL){ //Buscamos la posicion a insertar, siempre en un arbol perfecto es un recorrido iterativo a izquierda
        temp = temp->hijo_izquierdo;
    }
    TNodo nuevo = (TNodo) malloc(sizeof(struct nodo)); //reservamos la respectiva memoria de un nodo
    if(nuevo!=NULL){
        nuevo->entrada = aInsertar;
        temp->hijo_izquierdo = nuevo;
        nuevo->padre = temp;
        cola->cantidad_elementos++;
        nuevo->hijo_derecho = NULL;
        nuevo->hijo_izquierdo = NULL;
        reacomodarInsercion(nuevo, cola);//Reacomodamos una vez realizadas las referencias del nodo insertado
        return TRUE;
    }
    else{
        return FALSE;
    }
}

int buscarEInsertar(int nivel, TEntrada aInsertar, TNodo nodo , TColaCP cola){ //Insercion en caso de que el arbol NO sea perfecto
    int nivel_actual = nivel; // empezará en 1 el nivel
    int retorno = FALSE;
    if (nivel_actual < logaritmo(cola->cantidad_elementos)){ //logaritmo en base 2 de los elementos, nos devuelve el último nivel
        nivel_actual++;
        retorno = buscarEInsertar(nivel_actual, aInsertar, nodo->hijo_izquierdo, cola);//recorremos recursivamente hasta encontrar el anteúltimo nivel
        if (!retorno)
            retorno = buscarEInsertar(nivel_actual, aInsertar, nodo->hijo_derecho, cola);
    }
    else {
        if (nodo->hijo_izquierdo == NULL) {
            TNodo nuevo = crear_nodo(aInsertar, nodo);
            reacomodarInsercion(nuevo, cola);//Reacomodamos post crecion del nodo
            cola->cantidad_elementos++;
            nodo->hijo_izquierdo = nuevo;//Creamos nueva referencia del nodo creado
            retorno = TRUE;
        }
        else if (nodo->hijo_derecho == NULL) {
            TNodo nuevo = crear_nodo(aInsertar, nodo);
            reacomodarInsercion(nuevo, cola);
            nodo->hijo_derecho = nuevo;
            cola->cantidad_elementos++;
            retorno = TRUE;
        }
    }
    return retorno;
}



int cp_insertar(TColaCP cola, TEntrada aInsertar){
    int aRetornar = FALSE;
    if(cola==NULL){ //Caso cola nula
        exit(CCP_NO_INI);
    }
    if (cola-> cantidad_elementos == 0){ //caso en que la cola este vacia
        TNodo nuevo = (TNodo) malloc(sizeof(struct nodo));//Reservamos memoria para el nodo
        if(nuevo!=NULL){
            nuevo->entrada = aInsertar;
            cola ->raiz = nuevo;
            cola->raiz->padre = POS_NULA;
            cola ->cantidad_elementos++;
            nuevo->hijo_derecho = POS_NULA;
            nuevo->hijo_izquierdo = POS_NULA;
            aRetornar = TRUE;
        }
        else{
            return FALSE;
        }
    }
    else
        if (cola->cantidad_elementos == pow (2,(logaritmo(cola->cantidad_elementos) + 1)) - 1){ //caso en que le arbol sea perfecto
            aRetornar= insertarPerfecto(aInsertar,cola);
        }
        else
            aRetornar = buscarEInsertar(1, aInsertar, cola->raiz, cola);//Caso en que el arbol NO sea perfecto


    return aRetornar;
}








