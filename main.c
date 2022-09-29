#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Global.h"
#include "FuncoesDebug.h"
#include "PesquisaArquivos.h"

void help(){
    printf("->Execucao sem argumentos, abrindo <help>\n");
    printf("->Primeiro argumento: Caminho onde os arquivos desejados estao\n");
    printf("->Todos os outros argumentos serao interpretados como os nomes dos arquivos\n");
    printf("->Estes arquivos serao o subgrupo de pesquisa\n");

    printf("->Exemplo:\n");
    printf("->->\"./main.exe 'nome_do_diretorio' 'nome_arquivo_1.txt' 'nome_arquivo_2.txt' 'nome_arquivo_3.txt'\"");
}

char * colocar_string_maiusculo(char * TagName){
    int i = 0;
    for(i = 0; i < strlen(TagName); i++){
        TagName[i] = (char)toupper(*(TagName + i));
    }
    return TagName;
}

void ler_tag(){
    char *TagName = malloc(sizeof(char) * MAX_SIZE_TAG);
    printf("Digite a Tag: ");
    scanf("%s", TagName);
    printf("\n");

    TagName = colocar_string_maiusculo(TagName);


    if(strstr(TagName, "*")){
        if(pesquisa_tag_incompletas(TagName) == 0){
            printf("NENHUMA TAG ENCONTRADA\n");
        }
    }else{
        if(pesquisa_tag_completas(TagName)==0){
            printf("TAG '%s' NAO ENCONTRADA\n", TagName);
        }
        if(num_tags_encontradas > 0){
            pesquisando_expressao = 1;
            pesquisa_tags_calculo();
            if(num_tags_encontradas > 0){
                printf("EXPRESSAO INVALIDA\n");
                printf("%s\n", tags_nao_encontradas);
            }else{
                printf("EXPRESSAO VALIDA\n");
            }
            pesquisando_expressao = 0;
        }
    }
}


int main(int argc, char **argv){
    int opcao_selecionada = -1;
    num_tags_encontradas = 0;
    pesquisando_expressao = 0;

    if(argc <= 1){
        help();
        return 0;
    }
    
    organizar_argumentos(argv, argc);

    while(opcao_selecionada != 0){
        printf("----------------------------\n");
        printf("|      Menu Principal      |\n");
        printf("----------------------------\n");
        printf("|1 - Realizar uma Pesquisa |\n");
        printf("|0 - Sair                  |\n");
        printf("----------------------------\n");
        printf("Digite uma opcao: ");
        scanf("%d", &opcao_selecionada);
        printf("----------------------------\n");

        //printf("A opcao selecionada foi %d\n", opcao_selecionada);
        switch (opcao_selecionada)
        {
            case 0:
                printf("Saindo...");
                break;
            case 1:
                ler_tag();
                break;
            default:
                printf("Opcao invalida! Vamos tentar mais uma vez.\n");
                opcao_selecionada = -1;
                break;
        }
    }

    return 0;
}