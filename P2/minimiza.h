#ifndef MINIMIZA_H
#define MINIMIZA_H

#include "afnd.h"
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "types.h"
#include "estado.h"
#include "transicion.h"

AFND * minimiza(AFND * afnd);

AFND * crate_min_afd(AFND * afnd, List * lc);

void dividir_clase(afd, clase_actual, lista_clases);

List * division_inicial(AFND * afd);

Bool son_equivalentes(AFND * afd, int a, int b, List *lista_clases);

int  get_clase_transicion(AFND * afd, int estado, int simbolo_actual, List *lista_clases);

int get_clase(int destino, List * lista_clases);

#endif