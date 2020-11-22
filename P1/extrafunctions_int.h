#ifndef EXTRAFUNCTIONS_INT_H
#define EXTRAFUNCTIONS_INT_H


#include <stdlib.h>
#include <stdio.h>

/**
* Archivo creado para las listas de enteros
*/

int extraint_print(FILE *pf, const void  *a);

void * extraint_copy (const void* a);

void extraint_free(void * a);

int extraint_cmp(const void * a, const void * b);


#endif /* EXTRAFUNCTIONS_INT_H */
