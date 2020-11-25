#include "minimiza.h"
#include "afnd.h"
#include "list.h"
#include <stdio.h>
#include <string.h>
#include "extrafunctions_int.h"
#include "types.h"
#include "clases_de_estados.h"


AFND * minimiza(AFND * afd)
{
    AFND * afd_min;
    List * lista_clases = NULL;
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
   lista_clases = division_inicial(afd);
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
            clase = dividir_clase(afnd, clase_actual);
            /*if(clase != NULL){
                list_pushBack(lista_clases, clase);
                clase_free(clase);
            }*/
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

void dividir_clase(afd, clase_actual, lista_clases)
{
    int num_estados = list_size(list_getElementInPos(lista_clases, clase_actual));
    int  flag;
    List * lista_chequeados = NULL;
    clase * aux;

    lista_chequeados = list_new(extraint_free, extraint_copy, extraint_print, extraint_cmp);
    if(lista_chequeados == NULL) {
         return;
    }

    for(int estado_actual = 0 ; i< num_estados; i++){
           /* compruebo que no lo he cheackeado ya */
           flag = 0;
           for(int i = 0; i< listSize(lista_chequeados) && flag == 0;i++){
               if (estado_actual == list_getElementInPos(lista_chequeados, i)){
                   flag = 1;
               }
           }

           if(flag == 0){
                aux = clase_ini(estado_actual);
                for(int j = estado_actual + 1; j < num_estados; j++){
                    /* compruebo que no lo he cheackeado ya */
                    for(int i = 0; i< listSize(lista_chequeados) && flag == 0;i++){
                        if (estado_actual == list_getElementInPos(lista_chequeados, i)){
                            flag = 1;
                        }
                    }
                    if(flag == 0){
                        /*compruebo equivalencia*/
                        if(son_equivlaentes(afd, estado_actual, j, lista_clases)==TRUE){
                            clase_inserta(aux, j);
                            /* j pasa a estar checkeado*/
                            list_pushBack(lista_chequeados, j);
                        }
                        
                    }
                }
             list_pushBack(lista_clases, aux);
             clase_free(aux);
            } 
    }
    /* elimino la clase porque ya estan metidas las divisiones en la lista de clases y asi evito redundancia*/
    lista_destroy_element_inpos(lista_clases, clase_actual);
}

List * division_inicial(AFND * afd)
{
    List * lista_clases = NULL;
    clase * clase_final = NULL;
    clase * clase_normal = NULL;
    int tipo = -1;

    if(!afd){
        return NULL;
    }

    for(int i = 0; i < AFNDNumEstados(afd); i++ ){
         tipo = AFNDTipoEstadoEn(afd, i);
         if(tipo == FINAL || tipo == INICIAL_Y_FINAL){
             clase_inserta(clase_final, i);
         }else{
             clase_inserta(clase_normal, i);
         }
    }
    list_pushBack(lista_clases, clase_normal);
    list_pushBack(lista_clases, clase_final);

    return lista_clases;
}

Bool son_equivalentes(AFND * afd, int a, int b, List *lista_clases)
{
    int flag = 0;
    int x;
    int y;
    for(int simbolo_actual = 0; simbolo_actual < AFNDNumSimbolos(afd) && flag == 0; simbolo_actual++){
        x = get_clase_transicion(afd, a, simbolo_actual, lista_clases);
        y = get_clase_transicion(afd, b, simbolo_actual, lista_clases);
        if(x != y){
            flag = 1;
        }
    }
    if(flag == 1){
        return FALSE
    }
    return TRUE;
}

int  get_clase_transicion(AFND * afd, int estado, int simbolo_actual, List *lista_clases)
{
    int destino = -1;
    int num_estados = AFNDNumEstados(afd);

    for(int i = 0; i< num_estados && destino == -1; i++){
        if(AFNDTransicionIndicesEstadoiSimboloEstadof(afnd,estado_actual, simbolo_pos, i)==1){
            destino = i;
        }
    }
    if(destino == -1){
        return get_clase(estado, lista_clases);
    }
    return get_clase(destino, lista_clases);
}

int get_clase(int destino, List * lista_clases)
{
    int ret = -1;
    List * aux = NULL;
    for(int i = 0; i < listSize(lista_clases) && ret == -1; i++){
        aux = clase_get_lista(list_getElementInPos(lista_clases, i));
        for(j = 0; j< listSize(aux) && ret == -1; j++){
            if(list_getElementInPos(aux, j) == destino){
                ret = list_getElementInPos(aux, 0);
            }
        }
    }
    return ret;
}
