#include "clases_de_estados.h"
#include "list.h"
#include "extrafunctions_int.h"
#include "afnd.h"


struct _clase{
    List * id_estados; /*Lista de ids de los estados en el AFD original */ 
    int tipo; /* Entero que nos indica si es una clase de estados finales o no */ 
};

clase * clase_ini(int estado_ini, int tipo_estado)
{
    clase * c;

    if(estado_ini < 0){
        return NULL;
    }
    if(tipo_estado < INICIAL || tipo_estado >NORMAL){
        return NULL;
    }

    c  = calloc(1, sizeof(clase));
    if(!c){
        return NULL;
    }
    c->id_estados = list_new(extraint_free, extraint_copy, extraint_print, extraint_cmp);
    if(!(c->id_estados)){
        free(c);
        return NULL;
    }

    list_pushBack(c->id_estados, &estado_ini);
    c->tipo = tipo_estado;

    return c;
}

List * clase_get_lista(const clase * c)
{
    if(!c){
        return NULL;
    }
    return c->id_estados;
}

int clase_get_tipo(const clase * c)
{
    if(!c){
        return -1;
    }
    return c->tipo;
}

void clase_set_lista(clase * c, List * l)
{
    if(!c || !l){
        return;
    }
    if(c->id_estados != NULL){
        list_free(c->id_estados);
    }
    c->id_estados = l;
}

void clase_set_tipo(clase *c, int tipo)
{
    if(!c || tipo < 0){
        return;
    }
    c->tipo = tipo;
}

/*  funciones para la lista */ 
void clase_free(void *c)
{
    if(!c){
        return;
    }
    if(clase_get_lista(c)!= NULL){
        list_free(clase_get_lista(c));
    }
    free(c);
}

void  * clase_cpy(const void * c)
{
    clase * cpy;
    if(!c){
        return NULL;
    }
    cpy = clase_ini(*(int *)list_getElementInPos(clase_get_lista(c), 0), clase_get_tipo(c));
    if(!cpy){
        return NULL;
    }
    for(int i  = 1; i< listSize(clase_get_lista(c)); i++){
        list_pushBack(cpy->id_estados, list_getElementInPos(clase_get_lista(c), i));
    }
    cpy->tipo = clase_get_tipo(c);

    return (void *)cpy;
}

int clase_cmp(const void * e1, const void * e2)
{
    return 1;
}

int clase_print(FILE * pf, const void * e)
{
   return 1;
}
/******************************************************/

Status clase_inserta(AFND* afd, clase * c, int estado)
{
    int tipo_estado = -1;

    if(!afd || !c || estado < 0){
        return ERROR;
    }

    tipo_estado = AFNDTipoEstadoEn(afd, estado);
    if(tipo_estado == -1){
        return ERROR;
    }

    list_pushBack(clase_get_lista(c), &estado);
    if(clase_get_tipo(c)==tipo_estado){
        return OK;
    }
    else if(clase_get_tipo(c) == FINAL && tipo_estado == INICIAL_Y_FINAL){
        clase_set_tipo(c, INICIAL_Y_FINAL);
        return OK;
    }
    else if(clase_get_tipo(c) == NORMAL && tipo_estado == INICIAL){
        clase_set_tipo(c, INICIAL);
        return OK;
    }

    return OK;
}
 

/*
    funciones para añadir un id y actualizar el tipo si fuese necesario
*/ 


