#ifndef MINIMIZA_H
#define MINIMIZA_H


#include "afnd.h"
#include "list.h"
#include <stdio.h>
#include <string.h>
#include "types.h"


#define MAX_STATE_NAME 1024


AFND * AFNDMinimiza(AFND * afnd);

AFND * crate_min_afd(AFND * afnd, List * lc);

void dividir_clase(AFND * afd, int clase_actual,List * lista_clases);

List * division_inicial(AFND * afd, List * l);

Bool equivalentes(AFND * afd, int a, int b, List *lista_clases);

int  get_clase_transicion(AFND * afd, int estado, int simbolo_actual, List *lista_clases);

int get_clase(int destino, List * lista_clases);

List * get_transiciones_lista(AFND * afnd, List * estados, int simbolo_pos);

List * get_transiciones(AFND * afnd, int estado_actual, int simbolo_pos);

char * genera_nombre_lista(List * l);

char * genera_nombre_destino_clases(List * lc, List * id_estados);

List * prog2_breadthFirst_adapted (AFND *afd);



#endif