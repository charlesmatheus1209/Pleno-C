#ifndef PESQUISA_ARQUIVOS_H
#define PESQUISA_ARQUIVOS_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "Global.h"

char * criar_nome_diretorio_completo(char *filename);
void mostrar_arquivo(char *filename);
bool pesquisa_tag_completas();
int pesquisa_tag_incompletas(char *TagName);
void pesquisa_tags_calculo();
#endif