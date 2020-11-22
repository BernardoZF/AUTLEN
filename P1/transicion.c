#include "transicion.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct _transicion{
    char * simbolo;          /* Caracter de la transicion */
    int tipo;                /* Nos indica si existe o no transicion o si es una lambda transicion*/
    char * destino;          /* Nombre del estado destino de la transicion */
};

transicion * new_transicion(char *simbolo, int tipo, char * destino)
{
    transicion * t;
     t=(transicion *)calloc (1 , sizeof(transicion));
     if(!t){
         return NULL;
    }

    if(tipo != TRANSICION_NORMAL) {
         t->tipo = tipo;
         t->simbolo = NULL;
         t->destino = NULL;
         return t;
     }
     
     t->simbolo =(char *) calloc(strlen(simbolo) + 1, sizeof(char));
     strcpy(t->simbolo,  simbolo);
     t->tipo = tipo;
     t->destino = calloc(strlen(destino) + 1 , sizeof(char));
     strcpy(t->destino, destino);


    return t;
}

void transicion_free(void *t)
{
    if(!t){
        return;
    }
    free (((transicion *)t)->simbolo);
    free (((transicion *)t)->destino);
    free((transicion *)t);
}

char *transicion_get_simbolo(const transicion *t)
{
    if(!t){
        return NULL;
    }
    
    return t->simbolo;
}

int transicion_get_tipo(const transicion *t)
{
    if(!t){
        return -2;
    }
    
    return t->tipo;
}

char * transicion_get_destino(const transicion *t)
{
    if(!t){
        return NULL;
    }
    
    return t->destino;
}

void transicion_set_simbolo(transicion *t, char *simbolo)
{
    if(!t ){
        return ;
    }
    t->simbolo=(char *)realloc(t->simbolo, (strlen(simbolo) +1) * sizeof(char));
    strcpy(t->simbolo,  simbolo);
}

void transicion_set_tipo(transicion *t, int tipo)
{
    if(!t){
        return;
    }
    if(tipo != TRANSICION_INEXISTENTE && tipo != TRANSICION_NORMAL){
        return ;
    }
    t->tipo = tipo;
}

void transicion_set_destino(transicion *t, char * destino)
{
    if(!t || destino == NULL){
        return;
    }
    
    t->destino=(char *)realloc(t->destino, (strlen(destino +1) * sizeof(char)));
    strcpy(t->destino,  destino);
}

/*
* Sin implementar porque no la necesito
*/
int transicion_print(FILE *pf, const void * e)
{
    return 1;
}

void * transicion_copy(const void *e)
{
    if(!e) return NULL;

    return (void *)new_transicion(transicion_get_simbolo(e), transicion_get_tipo(e), transicion_get_destino(e));
}

int transicion_cmp(const void * e, const void* t)
{
    return 1;
}