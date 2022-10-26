#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "colacp.h"
#ifdef _WIN32
#include <windows.h>
#endif




typedef struct ciudad {
    char * nombre;
    float pos_x;
    float pos_y;
} * TCiudad;

typedef struct usuario {
    float pos_x;
    float pos_y;
} * TUsuario;

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



    free(usuario1);
    free(arr);
    cp_destruir();
    /*for (int iterador = 0; iterador <= 3; iterador++){
        printf("ciudad %d: %s, posicion: %f %f \n",iterador, arr[iterador]->nombre, arr[iterador]->pos_x, arr[iterador]->pos_y);
    }*/
    return 0;
}

