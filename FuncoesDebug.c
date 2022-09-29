#include "FuncoesDebug.h"

void mostrar_argumentos(char **argv, int argc){
    int i = 0; 
    for(int i = 0; i < argc; i++){
        printf("-> %s\n", *(argv + i));
    }
}

void organizar_argumentos(char **argv, int argc){
    int i = 0;

    nomeDiretorio = *(argv + 1);
    *(nomeDiretorio + strlen(nomeDiretorio)) = '/';
    *(nomeDiretorio + strlen(nomeDiretorio)-1) = '\0';
    filesRequired = (argv + 2);
    numeroDeArquivosRequeridos = argc - 2;

    if(strstr(nomeDiretorio, "\\")){
        for(i = 0; i < strlen(nomeDiretorio); i++){
            if(nomeDiretorio[i] == '\\'){
                nomeDiretorio[i] = '/';
            }
        }
        nomeDiretorio[i-2] = '\0';
    }
}

