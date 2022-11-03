#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "colacp.h"
#ifdef _WIN32
#include <windows.h>
#endif

float calcularDistancia(TCiudad ciudad, TUsuario us){
    return fabsf(ciudad->pos_x - us->pos_x) + fabsf(ciudad->pos_y - us->pos_y);
}

void mostrarAscendente(TCiudad* arr, int cont, TUsuario us){
        TColaCP nueva = crear_cola_cp(ordenAscendente);
        float* prioCiudad;
        for (int i = 0; i < cont; i++){
            TEntrada aCrear = (TEntrada)malloc(sizeof(struct entrada));
            prioCiudad = (float*)malloc(sizeof (float));
            *prioCiudad = calcularDistancia(arr[i], us);
            aCrear->clave = prioCiudad;
            aCrear->valor = (TValor) malloc(sizeof(char) * (strlen(arr[i]->nombre) + 1));
            strcpy(aCrear->valor,arr[i]->nombre);
            cp_insertar(nueva, aCrear);
        }
        cont = 0;

        while(cp_cantidad(nueva)  > 0){
            TEntrada aMostrar = cp_eliminar(nueva);
            printf("Nº%d: %s \n",cont, (char *) (aMostrar->valor));
            free(aMostrar->valor);
            free(aMostrar->clave);
            free(aMostrar);
            cont++;
        }
        cp_destruir(nueva, fEliminar);
    }

    void mostrarDescendente(TCiudad* arr, int cont, TUsuario us){
        TColaCP nueva = crear_cola_cp(ordenDescendente);
        float* prioCiudad;
        for (int i = 0; i < cont; i++){
            TEntrada aCrear = (TEntrada)malloc(sizeof(struct entrada));
            prioCiudad = (float*)malloc(sizeof (float));
            *prioCiudad = calcularDistancia(arr[i], us);
            aCrear->clave = prioCiudad;
            aCrear->valor = (TValor) malloc(sizeof(char) * (strlen(arr[i]->nombre) + 1));
            strcpy(aCrear->valor,arr[i]->nombre);
            cp_insertar(nueva, aCrear);
        }
        cont = 0;
        while(cp_cantidad(nueva) > 0){
            TEntrada aMostrar = cp_eliminar(nueva);
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
        TColaCP nueva = crear_cola_cp(ordenAscendente);
        TColaCP secundaria = crear_cola_cp(ordenAscendente);
        TCiudad c;
        float distanciaRecorrida=0;
        //chequear los malloc
        //cont-1 tiene que ser size
        for (int i = 0; i < size; i++) {
            TEntrada aCrear = (TEntrada) malloc(sizeof(struct entrada));
            prioCiudad = (float *) malloc(sizeof(float));
            *prioCiudad = calcularDistancia(arr[i], us);
            aCrear->clave = prioCiudad;
            aCrear->valor = arr[i];
            cp_insertar(nueva, aCrear);
        }
        int restantes = size;

        while (restantes > 0) {
            TEntrada temporal;
            temporal = cp_eliminar(nueva);

            restantes--;
            c = (TCiudad) temporal->valor;

            us->pos_x = c->pos_x;
            us->pos_y = c->pos_y;
            distanciaRecorrida = distanciaRecorrida + *((float*) (temporal->clave));
            printf("ciudad Numero %d: %s \n", size - restantes, (char*) ((TCiudad)temporal->valor)->nombre);
            free(temporal->clave);
            free(temporal);

            while (cp_cantidad(nueva)  != 0){
                temporal = cp_eliminar(nueva);
                *(float*)temporal->clave = calcularDistancia(temporal->valor, us);
                cp_insertar(secundaria, temporal);

            }

            TColaCP aux = secundaria;
            secundaria = nueva;
            nueva = aux;

        }
        printf("Total recorrido: %.1f\n",distanciaRecorrida);
        cp_destruir(nueva,fEliminar);
        cp_destruir(secundaria, fEliminar);
    }

int main(int argc, char* argv[]){
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif
    if (argc != 2){
        printf("error en args");
        exit(1);
    }
    FILE *archivo = fopen(argv[1],"r");
    if (archivo == NULL){
        printf("no se pudo abrir el archivo. \n");
        exit(1);
    }

    char temporal[32];
    int cont = 0;


    while(fgets(temporal,50,archivo)){
        cont++;
    }

    rewind(archivo);

    TUsuario usuario1 = (TUsuario) malloc(sizeof(struct usuario));

    fscanf(archivo,"%f;%f\n",&usuario1->pos_x,&usuario1->pos_y);

    TCiudad *arr = (TCiudad*) malloc(cont * sizeof(struct ciudad));
    for(int i = 0; i<cont;i++){
        arr[i] = (TCiudad) malloc(sizeof(struct ciudad));
        char* strCiudad = (char*) malloc(32* sizeof(char));
        arr[i]->nombre = strCiudad;
        fscanf(archivo,"%[^;];%f;%f\n",arr[i]->nombre, &arr[i]->pos_x, &arr[i]->pos_y);
    }
    int size;
    size = cont-1;
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

