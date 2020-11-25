#include "clases_de_estados.h"
#include "list.h"
#include "extrafunctions_int.h"
#include "afnd.h"


struct _clase{
    List * id_estados; /*Lista de ids de los estados en el AFD original */ 
    int tipo; /* Entero que nos indica si es una clase de estados finales o no */ 
}

clase * clase_ini(int estado_ini, int tipo_estado)
{
    clase * c;

    if(estado_ini < 0){
        return NULL;
    }
    if(tipo_estado < NORMAL || tipo_estado > INICIAL_Y_FINAL){
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

    list_pushBack(c->id_estados, estado_ini);
    c->tipo = tipo_estado;

    return c;
}

/*
    GETTERS Y SETTERS 
*/

/*  funciones para la lista */ 



/* 
    funciones para borrar un id que hay que llamar a la estructura lista 
    que hace falta crear unas funciones en esta
    y despues de llamar a la funcion creadaactualizar tipo de la clase 
*/ 

/*
    funciones para añadir un id y actualizar el tipo si fuese necesario
*/ 


