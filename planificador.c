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
            strcpy(aCrear->valor,arr[i]->nombre);//verificar si reserva memoria
            cp_insertar(nueva, aCrear);
        }
        cont = 0;
        while(nueva->cantidad_elementos  > 0){
            TEntrada aMostrar = cp_eliminar(nueva);
            printf("Nº%d: %s \n",cont, (char *) (aMostrar->valor));
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
            strcpy(aCrear->valor,arr[i]->nombre);//verificar si reserva memoria
            cp_insertar(nueva, aCrear);
        }
        cont = 0;
        while(nueva->cantidad_elementos > 0){
            TEntrada aMostrar = cp_eliminar(nueva);
            printf("Nº%d: %s \n",cont, (char*) aMostrar->valor);
            cont++;
        }
        cp_destruir(nueva, fEliminar);
    }

    void reducirHorasDeManejo(TCiudad* arr, int cont, TUsuario us){
        float* prioCiudad;
        TColaCP nueva = crear_cola_cp(ordenAscendente);

        for (int i = 0; i < cont; i++){




            for (int h = i - 1; h < cont; h++){
            TEntrada aCrear = (TEntrada)malloc(sizeof(struct entrada));
            prioCiudad = (float*)malloc(sizeof (float));
            *prioCiudad = calcularDistancia(arr[h], us);
            aCrear->clave = prioCiudad;
            aCrear->valor = (TValor) malloc(sizeof(char) * (strlen(arr[i]->nombre) + 1));
            strcpy(aCrear->valor,arr[h]->nombre);//verificar si reserva memoria
            cp_insertar(nueva, aCrear);
            }

            for (int k = 0; k < cont; k++){
                if (k == i){
                    TEntrada aMostrar = cp_eliminar(nueva);
                    printf("ciudad nro %d: %s \n",i, (char*) aMostrar->valor);
                }
                else
                    cp_eliminar(nueva);
            }
        }
        cp_destruir(nueva, fEliminar);
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

    //mostrarAscendente(arr, cont - 1, usuario1);
    reducirHorasDeManejo();
    free(usuario1);
    free(arr);


    return 0;


}

