#ifndef ESTADO_H
#define ESTADO_H

#include "List.h"
#include "transicion.h"


typedef struct _estado estado;


estado * new_estado(int id, char* nombre, int tipo);

void free_estado(void * e);

void * estado_cpy(const void* src);

int estado_get_id(const estado * e);

char * estado_get_nombre(const estado * e);

int estado_get_tipo(const estado* e);

List * estado_get_lista_estados(const estado * e);

List * estado_get_lista_transiciones(const estado * e);

void estado_set_id(estado * e, int id);

void estado_set_nombre(estado * e, char * nombre);

void estado_set_tipo(estado* e, int tipo);

void estado_set_lista_estados(estado * e, List * l);

int estado_cmp(const void * e1, const void * e2);

int estado_print(FILE * pf, const void * e);

Status estado_lista_combinaciones_push(estado * e, int id);

Status estado_lista_transiciones_push(estado * e, transicion * t);

#endif