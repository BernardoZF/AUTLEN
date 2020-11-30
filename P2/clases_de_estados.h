#ifndef CLASES_DE_ESTADOS_H
#define CLASES_DE_ESTADOS_H


#include "list.h"
#include "extrafunctions_int.h"
#include "afnd.h"

typedef struct _clase clase;

clase * clase_ini(int estado_ini, int tipo_estado);

List * clase_get_lista(const clase * c);

int clase_get_tipo(const clase * c);

void clase_set_lista(clase * c, List * l);

void clase_set_tipo(clase *c, int tipo);

void clase_free(void *c);

void  * clase_cpy(const void * c);

int clase_cmp(const void * e1, const void * e2);

int clase_print(FILE * pf, const void * e);

Status clase_inserta(AFND* afd, clase * c, int estado);

#endif /*CLASES_ESTADOS_H*/
