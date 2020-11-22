#ifndef TRANSFORMA_H
#define TRANSFORMA_H

#include "afnd.h"
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "types.h"
#include "estado.h"
#include "transicion.h"


#define PRIMER_ESTADO_INSERTADO -15


AFND * AFNDTransforma(AFND * afnd);


AFND *new_afd(AFND * afnd, List * le);


List * get_Ltrans(AFND * afnd, int id_estado);

List * get_Ltrans_lista(AFND *afnd, List * le);

char * genera_nombre_estado(estado * e);

char * genera_nombre_lista(List * l);

int genera_tipo_estado(AFND * afnd, estado * e);

int genera_tipo_lista(AFND * afnd, List *l, int ini);

List * get_transiciones(AFND * afnd, int estado_actual, int simbolo_pos);

List * get_transiciones_lista(AFND * afnd, List * estados, int simbolo_pos);



#endif
