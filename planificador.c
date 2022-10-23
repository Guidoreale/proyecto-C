#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 2000
void f(char *file1){

    char buffer[SIZE];
    FILE *archivo1 = fopen(file1,"r");
    int encontre = 0;
    char ch;
    char cadena[SIZE];
    int i = 0;
    int entero = 0;
    if(!archivo1){
        printf("Error al abrir el archivo\n");
    }

    else{



        fclose(archivo1);
    }

}



int main(int argc,char *args[])
{
    if (argc==2){
        f(args[1]);
    }
    else{
        printf("error en args\n");
    }

    return 0;
}