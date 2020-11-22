#ifndef TRANSICION_H
#define TRANSICION_H


#define TRANSICION_INEXISTENTE -1
#define TRANSICION_NORMAL 0

typedef struct _transicion transicion;

#include <stdio.h>



transicion * new_transicion(char *simbolo, int tipo, char * destino);

void transicion_free(void *t);

char *transicion_get_simbolo(const transicion *t);

int transicion_get_tipo(const transicion *t);

char * transicion_get_destino(const transicion *t);

void transicion_set_simbolo(transicion *t, char *simbolo);

void transicion_set_tipo(transicion *t, int tipo);

void transicion_set_destino(transicion *t, char * destino);

int transicion_print(FILE *pf, const void * e);

void * transicion_copy(const void *e);

int transicion_cmp(const void * e, const void* t);

#endif