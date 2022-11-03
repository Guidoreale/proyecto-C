#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "colacp.h"
#ifdef _WIN32
#include <windows.h>
#endif

float calcularDistancia(TCiudad ciudad, TUsuario us){ //Computa la distancia entre 2 ciudades
    return fabsf(ciudad->pos_x - us->pos_x) + fabsf(ciudad->pos_y - us->pos_y);
}

void mostrarAscendente(TCiudad* arr, int cont, TUsuario us){
        TColaCP nueva = crear_cola_cp(ordenAscendente);//se crea una nueva cola con orden de prioridad ascendente
        float* prioCiudad;
        for (int i = 0; i < cont; i++){//se inserta en la cola los nombres de las ciudades y su prioridad correspondiente
            TEntrada aCrear = (TEntrada)malloc(sizeof(struct entrada));
            prioCiudad = (float*)malloc(sizeof (float));
            *prioCiudad = calcularDistancia(arr[i], us);//se calcula la distancia de la ciudad al usuario y se almacena
            aCrear->clave = prioCiudad;
            aCrear->valor = (TValor) malloc(sizeof(char) * (strlen(arr[i]->nombre) + 1));//se guarda el nombre de la ciudad correspondiente
            strcpy(aCrear->valor,arr[i]->nombre);//se copia el nombre guardado
            cp_insertar(nueva, aCrear);
        }
        cont = 0;

        while(cp_cantidad(nueva)  > 0){
            TEntrada aMostrar = cp_eliminar(nueva);//se elimina la entrada que contiene el nombre de la siguiente ciudad en orden ascendente
            printf("Nº%d: %s \n",cont, (char *) (aMostrar->valor));
            free(aMostrar->valor);
            free(aMostrar->clave);
            free(aMostrar);
            cont++;
        }
        cp_destruir(nueva, fEliminar);
    }

    void mostrarDescendente(TCiudad* arr, int cont, TUsuario us){
        TColaCP nueva = crear_cola_cp(ordenDescendente);//se crea una nueva cola con orden de prioridad descendente
        float* prioCiudad;
        for (int i = 0; i < cont; i++){//se inserta en la cola los nombres de las ciudades y su prioridad correspondiente
            TEntrada aCrear = (TEntrada)malloc(sizeof(struct entrada));
            prioCiudad = (float*)malloc(sizeof (float));
            *prioCiudad = calcularDistancia(arr[i], us);//se calcula la distancia de la ciudad al usuario y se almacena
            aCrear->clave = prioCiudad;
            aCrear->valor = (TValor) malloc(sizeof(char) * (strlen(arr[i]->nombre) + 1));//se guarda el nombre de la ciudad correspondiente
            strcpy(aCrear->valor,arr[i]->nombre);//se copia el nombre guardado
            cp_insertar(nueva, aCrear);
        }
        cont = 0;
        while(cp_cantidad(nueva) > 0){
            TEntrada aMostrar = cp_eliminar(nueva);//se elimina la entrada que contiene el nombre de la siguiente ciudad en orden descendente
            printf("Nº%d: %s \n",cont, (char*) aMostrar->valor);
            free(aMostrar->valor);
            free(aMostrar->clave);
            free(aMostrar);
            cont++;
        }
        cp_destruir(nueva, fEliminar);
    }

    void reducirHorasDeManejo(TCiudad* arr, int size, TUsuario us){
        float* prioCiudad;
        TColaCP principal = crear_cola_cp(ordenAscendente);
        TColaCP secundaria = crear_cola_cp(ordenAscendente);
        TCiudad c;
        float distanciaRecorrida=0;

        for (int i = 0; i < size; i++) { //se inserta en la cola las ciudades, siendo la clave su distancia al usuario y siendo el valor una ciudad
            TEntrada aInsertar = (TEntrada) malloc(sizeof(struct entrada));
            prioCiudad = (float *) malloc(sizeof(float));
            *prioCiudad = calcularDistancia(arr[i], us);//se calcula la distancia de la ciudad a la posicion actual del usuario
            aInsertar->clave = prioCiudad;
            aInsertar->valor = arr[i];
            cp_insertar(principal, aInsertar);
        }
        int restantes = size;

        while (restantes > 0) {//se elimina una entrada(que contiene la ciudad proxima a visitar), se inserta en una cola secundaria y se cambia la referencia entre las colas
            TEntrada temporal;
            temporal = cp_eliminar(principal); //se elimina la entrada que contiene la ciudad mas proxima a visitar

            restantes--;
            c = (TCiudad) temporal->valor;

            us->pos_x = c->pos_x;//se actualiza la posicion del usuario
            us->pos_y = c->pos_y;//se actualiza la posicion del usuario
            distanciaRecorrida = distanciaRecorrida + *((float*) (temporal->clave));//se suma la distancia recorrida en el ultimo movimiento del usuario
            printf("Ciudad Numero %d: %s \n", size - restantes, (char*) ((TCiudad)temporal->valor)->nombre);
            free(temporal->clave);
            free(temporal);

            while (cp_cantidad(principal) != 0){//se pasan las entradas restantes en la cola principal a la cola secundaria, recalculando su distancia
                temporal = cp_eliminar(principal); //se asigna a la variable "temporal" una principal entrada que contenga la ciudad mas proxima a visitar por el usuario
                *(float*)temporal->clave = calcularDistancia(temporal->valor, us);
                cp_insertar(secundaria, temporal); //se inserta en la cola secundaria las entradas restantes recalculando su distancia con respecto a la principal posicion del usuario

            }

            TColaCP aux = secundaria; //se intercambian las referencias de las colas
            secundaria = principal;//se intercambian las referencias de las colas
            principal = aux;//se intercambian las referencias de las colas

        }
        printf("Total recorrido: %.1f\n",distanciaRecorrida);
        cp_destruir(principal, fEliminar);
        cp_destruir(secundaria, fEliminar);
    }

int main(int argc, char* argv[]){
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif
    if (argc != 2){
        printf("error en args");//se muestra si los argumentos son incorrectos
        exit(1);
    }
    FILE *archivo = fopen(argv[1],"r");
    if (archivo == NULL){
        printf("no se pudo abrir el archivo. \n");
        exit(1);
    }

    char temporal[32];
    int cantLineas = 0;


    while(fgets(temporal,50,archivo)){
        cantLineas++;
    }//se lee el archivo y se almacenan la cantidad de filas(1 de la posicion del usuario y el resto ciudades)

    rewind(archivo);

    TUsuario usuario1 = (TUsuario) malloc(sizeof(struct usuario));

    fscanf(archivo,"%f;%f\n",&usuario1->pos_x,&usuario1->pos_y);//se lee y almacena la posicion del usuario

    TCiudad *arr = (TCiudad*) malloc((cantLineas - 1) * sizeof(TCiudad));
    for(int i = 0; i < cantLineas; i++){
        arr[i] = (TCiudad) malloc(sizeof(struct ciudad));
        char* strCiudad = (char*) malloc(32* sizeof(char)); //se guarda espacio para el string, maximo permitido 32 char
        arr[i]->nombre = strCiudad;
        fscanf(archivo,"%[^;];%f;%f\n",arr[i]->nombre, &arr[i]->pos_x, &arr[i]->pos_y);
    }
    int size;
    size = cantLineas - 1; //se guarda la cantidad de ciudades
    fclose(archivo);


    int leerNumero = 0;
    while(leerNumero!=4){
        printf("Ingrese una accion a realizar:\n");
        printf("1: Mostrar ascendente\n");
        printf("2: Mostrar descendente\n");
        printf("3: Reducir horas de manejo\n");
        printf("4: Salir\n");

        scanf("%i",&leerNumero);
        if(leerNumero != 4)
        switch (leerNumero) {
            case 1:
                    mostrarAscendente(arr, size, usuario1);
                    printf(" \n");
                break;
            case 2:
                    mostrarDescendente(arr, size, usuario1);
                    printf(" \n");
                break;
            case 3:
                    reducirHorasDeManejo(arr, size, usuario1);
                    printf(" \n");
                break;
            case 4:
                for(int i = 0;i<size;i++){
                    free(arr[i]->nombre);
                    free(arr[i]);
                }
                free(usuario1);
                free(arr);
                break;

            default:
                printf("Fuera de rango\n");
                break;
        }
    }


    return 0;


}

