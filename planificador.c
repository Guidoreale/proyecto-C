#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "colacp.h"
#ifdef _WIN32
#include <windows.h>
#endif




void vaciar(char temp[]){
    for (int i = 0; i < 50; i++){
        temp[i] = '\0';
    }
}

void cargarNombre(char temp[], int contador, TCiudad arreglo ){
    int k = strlen(temp) + 1;
    arreglo[contador].nombre = (char*) malloc (k * sizeof(char));
    strcpy(arreglo[contador].nombre, temp);
}
void cargarPosX(float temp, int contador, TCiudad arreglo){
    arreglo[contador].pos_x = temp;
}
void cargarPosY(float temp, int contador, TCiudad arreglo){
    arreglo[contador].pos_y = temp;
}

void cargarXUsuario(float temp, TUsuario usuario){
    usuario->pos_x = temp;
}

void cargarYUsuario(float temp, TUsuario usuario){
    usuario->pos_y = temp;
}

float calcularDistancia(TCiudad ciudad, TUsuario us){
    double aRetornar = 0;
    aRetornar =  fabs(ciudad->pos_x - us->pos_x) + fabs(ciudad->pos_y - us->pos_y);
    return (float) aRetornar;
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
        while(nueva->cantidad_elementos  > 0){
            TEntrada aMostrar = cp_eliminar(nueva);
            printf("Nº%d: %s \n",cont, (char *) (aMostrar->valor));
            cont++;
        }
        //cp_destruir(nueva, fEliminar);
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
        while(nueva->cantidad_elementos > 0){
            TEntrada aMostrar = cp_eliminar(nueva);
            printf("Nº%d: %s \n",cont, (char*) aMostrar->valor);
            cont++;
        }
        //cp_destruir(nueva, fEliminar);
    }

    TCiudad buscarCiudad(TCiudad*arr, int cont, char* nombre){
        TCiudad aRetornar;
        for (int i = 0; i < cont - 1; i++)
            if (strcmp(nombre, arr[i]->nombre) == 0)
                aRetornar = arr[i];

        return aRetornar;
    }

    void reducirHorasDeManejo(TCiudad* arr, int cont, TUsuario us){
        float* prioCiudad;
        TCiudad ciudadTemp;
        ciudadTemp = (TCiudad) malloc(sizeof(struct ciudad));
        TColaCP nueva = crear_cola_cp(ordenAscendente);
        TColaCP secundaria = crear_cola_cp(ordenAscendente);

        for (int i = 0; i < cont - 1; i++) {
            TEntrada aCrear = (TEntrada) malloc(sizeof(struct entrada));
            prioCiudad = (float *) malloc(sizeof(float));
            *prioCiudad = calcularDistancia(arr[i], us);
            aCrear->clave = prioCiudad;
            aCrear->valor = (TValor) malloc(sizeof(char) * (strlen(arr[i]->nombre) + 1));
            strcpy(aCrear->valor,arr[i]->nombre);
            cp_insertar(nueva, aCrear);
        }

        int restantes = cont ;
        int iterador;
        while (restantes > 1) {
            iterador = 0;

            TEntrada temporal = (TEntrada) malloc(sizeof(struct entrada));
            temporal = cp_eliminar(nueva);
            restantes--;

            while (iterador < cont - 1) {
                if (strcmp(temporal->valor, arr[iterador]->nombre) == 0) {
                    us->pos_x = arr[iterador]->pos_x;
                    us->pos_y = arr[iterador]->pos_y;
                }
                iterador++;
            }
            printf("ciudad Numero%d: %s \n", restantes - cont, (char*)temporal->valor);

            while (nueva->cantidad_elementos != 0){
                temporal = cp_eliminar(nueva);
                ciudadTemp = buscarCiudad(arr, cont, temporal->valor);
                *prioCiudad = calcularDistancia(ciudadTemp, us);
                temporal->clave = prioCiudad;
                cp_insertar(secundaria, temporal);
            }

            iterador = 0;
            temporal = cp_eliminar(secundaria);
            restantes--;
            while (iterador < cont - 1) {
                if (strcmp(temporal->valor, arr[iterador]->nombre) == 0) {
                    us->pos_x = arr[iterador]->pos_x;
                    us->pos_y = arr[iterador]->pos_y;
                }
                iterador++;
            }
            printf("ciudad Numero%d: %s \n", restantes - cont, (char*)temporal->valor);

            while (secundaria->cantidad_elementos != 0){
                temporal = cp_eliminar(secundaria);
                ciudadTemp = buscarCiudad(arr, cont, temporal->valor);
                *prioCiudad = calcularDistancia(ciudadTemp, us);
                temporal->clave = prioCiudad;
                cp_insertar(nueva, temporal);
            }
        }
        if (restantes > 0) {
            printf("ciudad Numero%d: %s", restantes - cont, (char*)cp_eliminar(nueva)->valor);
        }
        //cp_destruir(nueva,fEliminar);
        //cp_destruir(secundaria, fEliminar);
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
    fclose(archivo);

    printf("Ingrese una accion a realizar:\n");
    printf("1: Mostrar ascendente\n");
    printf("2: Mostrar descendente\n");
    printf("3: Reducir horas de manejo\n");
    printf("4: Salir\n");
    int leerNumero;
    scanf("%i",&leerNumero);
    switch (leerNumero) {
        case 1:
            mostrarAscendente(arr, cont - 1, usuario1);
            break;
        case 2:
            mostrarDescendente(arr, cont - 1, usuario1);
            break;
        case 3:
            reducirHorasDeManejo(arr, cont, usuario1);
            break;
        case 4:
            //cp_destruir(arr,fEliminar);
            break;

        default:
            printf("Fuera de rango");
            break;
    }
    free(usuario1);
    free(arr);


    return 0;


}

