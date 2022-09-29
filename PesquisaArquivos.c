#include "PesquisaArquivos.h"
#include <string.h>
#include <dirent.h>

#include <ctype.h>
#include "Global.h"

//--------------------------------------------------------------------------------------------------------------------

char * criar_nome_diretorio_completo(char *filename){
    /*
     * Essa função serve para criar o nome completo do diretório desejado.
     * Este nome é formado por "Nome_da_pasta/Nome_do_arquivo"
     */
    
    int sizeOfPathName = strlen(nomeDiretorio);
    int sizeOfFilename = strlen(filename);

    char * filePath = malloc(sizeof(char) * (sizeOfPathName + sizeOfFilename + 2));
    int i = 0; 

    

    for(i = 0; i < (sizeOfPathName + sizeOfFilename + 1); i++){
        if(i < sizeOfPathName){
            *(filePath + i) = nomeDiretorio[i];
        }else if (i == sizeOfPathName){
            *(filePath + i) = '/';
        }
        else{
            *(filePath + i) = filename[i - sizeOfPathName - 1];
        }   
    }
    *(filePath + i) = '\0';

    return filePath;
}

//--------------------------------------------------------------------------------------------------------------------

void mostrar_arquivo(char *filename){
    /*
     * Essa função percorre todo o arquivo mostrando todas as tags, usada apenas para debug
     */
    int sizeOfPathName = strlen(nomeDiretorio);
    int sizeOfFilename = strlen(filename);

    char * filePath = criar_nome_diretorio_completo(filename);

    FILE *file = fopen(filePath, "r");

    while(!feof(file)){
        char linha[MAX_SIZE_LINE];
        fgets(linha, MAX_SIZE_LINE, file);
        printf("%s", linha);
    } 
}

//--------------------------------------------------------------------------------------------------------------------

char * tagDaLinha(char linha[]){
    /*
     * Essa função é responsável por retirar da linha somente o nome da Tag
     */
    char * tag_da_linha = (char *)malloc(MAX_SIZE_TAG * sizeof(char));
    int i;
    for(i = 0; i < strlen(linha); i++){
        if(linha[i] == ';'){
            tag_da_linha[i] = '\0';
            return tag_da_linha;
        }else{
            tag_da_linha[i] = linha[i];
        }
    }

    return tag_da_linha;
}

//--------------------------------------------------------------------------------------------------------------------

bool encontrar_todos_tags_no_arquivo(char *TagName, char *caminho_do_arquivo){
    /*
     * Essa função pesquisa todas as tags que seguem o padrão pedido, dentro de um determinado arquivo
     */
    bool TagEncontrada = false;
    FILE * arquivo = fopen(caminho_do_arquivo, "r");

    while(!feof(arquivo)){
        char linha[MAX_SIZE_LINE];
        fgets(linha, MAX_SIZE_LINE, arquivo);
        char * Tag = tagDaLinha(linha);
        char * p = strstr(Tag, TagName);
        if(p != NULL){
            printf("%s\n", linha);
            TagEncontrada = true;
        }
    }

    fclose(arquivo);

    return TagEncontrada;
}

//--------------------------------------------------------------------------------------------------------------------

char * formata_novo_tag(char * TagName){
    /*
     * Essa função identifica o padão da tag de pesquisa e cria uma nova Tag que será a substring pesquisada
     * Padrão 1: *codigo
     * Padrão 2:  codigo*
     * Padrão 3: *codigo*
     * Caso a tag não siga nenhum desses 3 padrões, o usuário será informado
     */
    int i = 0;
    int padrao = 0;
    if(TagName[0] == '*' && TagName[strlen(TagName) - 1] != '*'){
        char * NewTag = (char *)malloc(sizeof(char)*strlen(TagName) - 1);
        for(i = 1; i < strlen(TagName); i++){
            NewTag[i-1] = TagName[i];
        }
        NewTag[i-1] = '\0';
        return NewTag;
    }else if(TagName[0] != '*' && TagName[strlen(TagName) - 1] == '*'){
        char * NewTag = (char *)malloc(sizeof(char)*strlen(TagName) - 1);
        for(i = 0; i < strlen(TagName) - 1; i++){
            NewTag[i] = TagName[i];
        }
        NewTag[i] = '\0';
        return NewTag;
    }else if(TagName[0] == '*' && TagName[strlen(TagName) - 1] == '*'){
        char * NewTag = (char *)malloc(sizeof(char)*strlen(TagName) - 2);
        for(i = 1; i < strlen(TagName)-1; i++){
            NewTag[i-1] = TagName[i];
        }
        NewTag[i-1] = '\0';
        return NewTag;
    }else{
        printf("Tag fora do padrao adequado\n");
        return "";
    }
}

//--------------------------------------------------------------------------------------------------------------------

int pesquisa_tag_incompletas(char *TagName){
    /*
     * Essa função pesquisa as tags que seguem um padrão específico (utilização do *)
     */
    int qtd_encontradas = 0;
    char *MyNewTag = formata_novo_tag(TagName);

    int i = 0;
    bool tagEncontrado = false;
    if(filesRequired[0] == NULL){ //O usuário não mencionou o subgrupo de arquivos
        DIR * diretorio = opendir(nomeDiretorio);
        struct dirent * arquivo_no_diretorio;

        while((arquivo_no_diretorio = readdir(diretorio)) != NULL){
            char * p = strstr(arquivo_no_diretorio->d_name, "expcal.txt"); //Confere se o arquivo não é um arquivo de calculo
            if(arquivo_no_diretorio->d_name[0] != '.' && ( p == NULL)){ // Indica que o arquivo não começa por '.', ou seja, é um arquivo válido
                char * caminho_do_arquivo = criar_nome_diretorio_completo(arquivo_no_diretorio->d_name);
                //printf("Pesquisando no Arquivo '%s'\n", caminho_do_arquivo);
                if(encontrar_todos_tags_no_arquivo(MyNewTag, caminho_do_arquivo) == true){
                    tagEncontrado = true;
                    qtd_encontradas++;
                }
            }
        }
        
    }else{ // O usuário mencionou um subgrupo específico de arquivos
        
        for(i = 0; i < numeroDeArquivosRequeridos; i++){ // Percorre somente o subgrupo de arquivos desejados
            char * caminho_do_arquivo = criar_nome_diretorio_completo(filesRequired[i]);
            // printf("Pesquisando no Arquivo '%s'\n", caminho_do_arquivo);
            if(encontrar_todos_tags_no_arquivo(MyNewTag, caminho_do_arquivo) == true){
                tagEncontrado = true;
                qtd_encontradas++;
            }
        }
    }
    return qtd_encontradas;
}

//--------------------------------------------------------------------------------------------------------------------

bool encontrar_tag_no_arquivo(char *TagName, char *caminho_do_arquivo,  char* nome_arquivo){
    /*
     * Essa função procura um determinado Tag dentro de um arquivo
     */
    
    FILE * arquivo = fopen(caminho_do_arquivo, "r");

    while(!feof(arquivo)){
        char linha[MAX_SIZE_LINE];
        char linha_anterior[MAX_SIZE_LINE];
        fgets(linha, MAX_SIZE_LINE, arquivo);
        char * Tag = tagDaLinha(linha);
        
        if(strcmp(Tag, TagName) == 0){
            if(strcmp(linha_anterior, linha) != 0){
                if(strstr(nome_arquivo, "cal") ){
                    printf("DADOS DO CALCULO:\n");
                }
                printf("%s\n", linha);
                return true;
            }
        }
        strcpy(linha_anterior, linha);
    }

    fclose(arquivo);

    return false;
}

//--------------------------------------------------------------------------------------------------------------------

int separa_tags_expressao(char * expressao){
    /*
     * Essa função percorre toda a expressão do cálculo e 
     * separa apenas as tags que fazem parte dos cálculo
     */
    char c = '.';
    int numero_de_tags = 0;
    tags_encontradas = (char*)malloc(sizeof(char)*MAX_SIZE_LINE);
    int i = 0, k = 0;

    while (c != '\0'){
        c = expressao[i++];  
        if(c == '@'){
            do{
                c = expressao[i++]; 
                if((isalpha(c) || c == '_' || isalnum(c))){
                    tags_encontradas[k++] = c;
                }                    
            }while(isalpha(c) || c == '_' || isalnum(c));
            tags_encontradas[k++] = ' ';
            numero_de_tags++;
        }
    }
    tags_encontradas[k] = '\0';
    return numero_de_tags;
}

//--------------------------------------------------------------------------------------------------------------------

void pesquisa_expressao_calculo(char * TagName){ 
    /*
     * Essa função serve para encontrar a expressão de um determinado tag de cálculo
     */

    char * newTag = (char*)malloc(sizeof(char)*strlen(TagName)+2);
    int i = 0;
    newTag[0] = '#';
    newTag[1] = ' ';
    for(i = 2; i < strlen(TagName) + 2; i++){
        newTag[i] = TagName[i-2];
    }
    newTag[i] = '\0';
    
    char * filename = criar_nome_diretorio_completo("expcal.txt");
    FILE * file = fopen(filename, "r");
    if(file == NULL){
        printf("Arquivo nao encontrado");
        return;
    }
    
    while(!feof(file)){
        char linha[MAX_SIZE_LINE];
        char linha1[MAX_SIZE_LINE];
        //printf("%s", linha);

        fgets(linha, MAX_SIZE_LINE, file);

        if(strstr(linha, newTag)){
            printf("EXPRESSAO:\n"); //Aqui ne inicia a exibição da EXPRESSÃO
            printf("%s", linha);
            fgets(linha, MAX_SIZE_LINE, file);
            printf("%s", linha);
            fgets(linha1, MAX_SIZE_LINE, file);
            printf("%s\n\n", linha1);

            num_tags_encontradas = separa_tags_expressao(linha);
            fclose(file);
            return;
        }

    }
    fclose(file);

}

//--------------------------------------------------------------------------------------------------------------------

void pesquisa_tags_calculo(){ // Essa funcão pesquisa todas as tags que formam a expressão do cálculo
	char* tag;
    tags_nao_encontradas = (char*)malloc(sizeof(char)*MAX_SIZE_LINE);
    int k = 0, i = 0;

    printf("TAGS DA EXPRESSAO:\n");

    //A funcão STRTOK está sendo usada para separar as tags encontradas na expressão
	
    tag = strtok(tags_encontradas, " ");
	if(pesquisa_tag_completas(tag) == 0){
        for(i = 0; i < strlen(tag); i++){
                tags_nao_encontradas[k++] = tag[i];
            }
            tags_encontradas[k] = ' ';
    }else{
        num_tags_encontradas--;
    }

	while ((tag = strtok(NULL, " ")) != NULL){
        if(pesquisa_tag_completas(tag) == 0){
            for(i = 0; i < strlen(tag); i++){
                tags_nao_encontradas[k++] = tag[i];
            }
            tags_encontradas[k] = ' ';
        }else{
            num_tags_encontradas --;
        }
    }
    tags_encontradas = '\0';
}

//--------------------------------------------------------------------------------------------------------------------

bool pesquisa_tag_completas(char * TagName){
    /*
     * Essa função serve encontrar tags completas (sem *) dentro dos arquivos
     */

    int i = 0;
    bool tagEncontrado = false;
    if(filesRequired[0] == NULL){ //O usuário não mencionou o subgrupo de arquivos
        DIR * diretorio = opendir(nomeDiretorio);
        struct dirent * arquivo_no_diretorio;

        while((arquivo_no_diretorio = readdir(diretorio)) != NULL){ // Este loop percorre todos os diretórios da pasta indicada

            char * p = strstr(arquivo_no_diretorio->d_name, "expcal.txt"); //Confere se o arquivo não é um arquivo de calculo

            if(arquivo_no_diretorio->d_name[0] != '.' && ( p == NULL) ){ // Indica que o arquivo não começa por '.', ou seja, é um arquivo válido
                char * caminho_do_arquivo = criar_nome_diretorio_completo(arquivo_no_diretorio->d_name);
             
                if(encontrar_tag_no_arquivo(TagName, caminho_do_arquivo, arquivo_no_diretorio->d_name) == true){
                    tagEncontrado = true;
                    if(strstr(arquivo_no_diretorio->d_name, "cal") && pesquisando_expressao == 0){
                        //Caso a tag encontrada seja uma Tag de Calculo, começa a pesquisa por seus componentes
                        pesquisa_expressao_calculo(TagName);
                    }
                    break;
                }
            }
        }
        
    }else{ // O usuário mencionou um subgrupo específico de arquivos
        
        for(i = 0; i < numeroDeArquivosRequeridos; i++){ // Percorre somente o subgrupo de arquivos desejados
            char * caminho_do_arquivo = criar_nome_diretorio_completo(filesRequired[i]); 
            
            if(encontrar_tag_no_arquivo(TagName, caminho_do_arquivo, filesRequired[i]) == true){
                tagEncontrado = true;

                if(strstr(filesRequired[i], "cal") && pesquisando_expressao == 0 ){
                    //Caso a tag encontrada seja uma Tag de Calculo, começa a pesquisa por seus componentes
                    pesquisa_expressao_calculo(TagName);
                }
                break;
            }
        }
    }
    return tagEncontrado;
}

//--------------------------------------------------------------------------------------------------------------------