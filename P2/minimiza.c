#include "minimiza.h"
#include "estado.h"
#include "transicion.h"
#include "afnd.h"
#include "list.h"
#include <stdio.h>
#include <string.h>
#include "extrafunctions_int.h"
#include "types.h"
#include "clases_de_estados.h"


AFND * minimiza(AFND * afnd)
{
    AFND * afd_min;
    List * lista_clases;
    int n_clases;
    clase * clase = NULL;

    
    /*Se puede hacer pero no es necesario al 100%*/
    /* simplifico los estados inaccesibles busqueda en anchura los que no se pueda acceder se eliminan*/


    /* algoritmo de reduccion de estados indistinguibles son a los que con todas sus transiciones van a la mismas clases
    */

   /*   
        Step 1 − All the states Q are divided in two partitions − final states and non-final states and are denoted by P0.
        All the states in a partition are 0th equivalent. Take a counter k and initialize it with 0.
    */
   lista_clases = division_inicial(afnd);
   if(lista_clases == NULL) {
       return NULL;
   }


   /*   
        Step 2 − Increment k by 1. For each partition in Pk, divide the states in Pk into two partitions if they are k-distinguishable.
        Two states within this partition X and Y are k-distinguishable if there is an input S such that δ(X, S) and δ(Y, S) are (k-1)-distinguishable.
   */
   do{
      n_clases =  listSize(lista_clases);
        for(int clase_actual = 0; clase_actual < listSize(lista_clases); clase_actual ++){
            clase = divisor_clase(afnd, clase_actual);
            if(clase != NULL){
                list_pushBack(lista_clases, clase);
                clase_free(clase);
            }
        }
   }while(n_clases != listSize(lista_clases));
   /*  Step 3 − If Pk ≠ Pk-1, repeat Step 2, otherwise go to Step 4.*/
   
   /* Step 4 − Combine kth equivalent sets and make them the new states of the reduced DFA. */ 
   afd_min = crate_min_afd(afnd, lista_clases);

   /*liberar memoria*/

   return afd_min;
}


AFND * crate_min_afd(AFND * afnd, List * lc)
{
    AFND * afd_min;
    char * nombre;
    /*CDE*/

    afd_min = AFNDNuevo("determinista", listSize(lc), AFNDNumSimbolos(afnd));
    /*CDE*/
    
    for(int i = 0;i<AFNDNumSimbolos(afnd); i++){
        AFNDInsertaSimbolo(afd_min, AFNDSimboloEn(afnd, i));
    }

    for(int i = 0 ; i<listSize(lc); i++){
        nombre = genera_nombre_lista(clase_get_lista(list_getElementInPos(lc, i))); // lo mismo hay que liberar memoria de esto
        AFNDInsertaEstado(afd_min, nombre, clase_get_tipo(list_getElementInPos(lc, i)));
    }

    for(int estado_actual = 0 ; estado_actual< listSize(lc); estado_actual++ ){
        for(int simbolo = 0; simbolo < AFNDNumSimbolos(); simbolo++){
            char * destino = genera_nombre_lista(get_transiciones_lista(afnd, list_getElementInPos(lc, estado_actual), simbolo));
            nombre = genera_nombre_lista(clase_get_lista(list_getElementInPos(lc, estado_actual)));
            AFNDInsertaTransicion(afd_min, nombre, AFNDSimboloEn(simbolo), destino);
        }
    }

    return afd_min;
}

/* FUNCIONES  DIVISION_INICIAL Y DIVISOR_CLASE  Y LAS NECESEARIAS PARA SU FUNCIONAMIENTO*/ 