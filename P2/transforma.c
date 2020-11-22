#include "transforma.h"
#include "estado.h"
#include "transicion.h"
#include "afnd.h"
#include "list.h"
#include <stdio.h>
#include <string.h>
#include "extrafunctions_int.h"
#include "types.h"


AFND * AFNDTransforma(AFND * afnd)
{
    List * lista_estados= NULL;                 /* Lista final de estados a insertar al afd */
    List * lista_idestados_aux= NULL;           /* Lista auxiliar en la que se almacenan */
    List * id_estados= NULL;
    estado * e = NULL;
    transicion *t= NULL;
    int flag = 0;
    char * destino =NULL;
    char * aux= NULL;

    if(!afnd){
        return NULL;
    }
    /*creamos lista de estados*/
    lista_estados = list_new(free_estado, estado_cpy, estado_print, estado_cmp);

    /*Creamos el estado inicial */
    e = new_estado(AFNDIndiceEstadoInicial(afnd),
                    AFNDNombreEstadoEn(afnd, AFNDIndiceEstadoInicial(afnd)),
                    INICIAL);

    /*Solo miramos Ltrans porque si hubiesen otras transiciones que hiciesen
    que este en 2 estados a la vez se crearia otro estado y solo quiero el inicial*/
    lista_idestados_aux = get_Ltrans(afnd, estado_get_id(e));

    /* Generamos sus parametros para insertar el inicio del nuevo afd*/
    estado_set_lista_estados(e, lista_idestados_aux); //e->lista_estados = lista pasada
    aux = genera_nombre_estado(e);
    estado_set_nombre(e, aux);
    estado_set_tipo(e, genera_tipo_lista(afnd, lista_idestados_aux, INICIAL));

    /*Una vez creados yy asignados los componentes del
     estado inicial lo insertamos en la lista de estados*/
    list_pushBack(lista_estados, e);

    /*no hace falta liberar la lista porque ya la libera el estado free*/
    free_estado(e);
    free(aux);
   

    /*Bucle principal Condicion de parada (no se generan estados nuevo)*/
    for(int estado_actual = 0; estado_actual < listSize(lista_estados);estado_actual++){

        /*miro todas las transiciones para cada simbolo del afnd*/
        for (int simbolo_actual= 0; simbolo_actual<AFNDNumSimbolos(afnd); simbolo_actual++){

                id_estados = get_transiciones_lista(afnd, estado_get_lista_estados(list_getElementInPos(lista_estados, estado_actual)), simbolo_actual); 
                lista_idestados_aux= get_Ltrans_lista(afnd, id_estados);

                /*Si la lista esta tiene menos de 1 elemento es que no hay ninguna transicion con ese simbolo*/
                if(listSize(lista_idestados_aux)<1){
                    e = list_getElementInPos(lista_estados, estado_actual);
                    t = new_transicion("NO_EXISTE_TRANSICION", TRANSICION_INEXISTENTE,"NO_EXISTE_TRANSICION");

                    /*Aqui si se libera porque no se le asigna al estado*/
                    list_free(lista_idestados_aux);
                    estado_lista_transiciones_push(e, t);
                    transicion_free(t);
                    
                }else{

                    /*Obtengo en que estado estoy actualmente es una asignacion con lo cual
                     no hay que liberar memoria ya que no se reserva nueva memoria
                    */
                    e = list_getElementInPos(lista_estados, estado_actual);
                    /*Genero el nombre del estado final*/

                    destino = genera_nombre_lista(lista_idestados_aux);
                   /*  Usado para debuggear
                     printf("\n+++++++++++++++++destino recien creado: %s\n", destino);
                   */

                  /*Creamos la transicion a insertar */
                    t = new_transicion(AFNDSimboloEn(afnd, simbolo_actual), TRANSICION_NORMAL, destino);
                    if(!t){
                        free(destino);
                        list_free(lista_estados);
                        list_free(id_estados);
                        list_free(lista_idestados_aux);
                        return NULL;
                    }

                    /*  Usado para debuggear
                        printf("*+*+*+*++*+*+*+*+*+**++*%s\n", transicion_get_destino(t));
                    */

                    if(estado_lista_transiciones_push(e, t)==ERROR){
                        transicion_free(t);
                        free(destino);
                        list_free(lista_estados);
                        list_free(id_estados);
                        list_free(lista_idestados_aux);
                        return NULL;
                    }

                    flag = 0;
                    /*Bucle para comprobar si el  estado destino ya existe en la lista de estados*/
                    for(int k = 0;k<listSize(lista_estados) && flag == 0;k++){
                        /*  Usado para debuggear
                            printf("%s, %s\n", estado_get_nombre(list_getElementInPos(lista_estados, k)), destino);
                        */
                        if(strcmp(estado_get_nombre(list_getElementInPos(lista_estados, k)), destino) == 0){
                            flag = 1;
                        }
                    }

                    /*  Usado para debuggear
                        printf("%d\n" ,flag);
                    */

                    /*Si no existe se inserta*/
                    if(flag == 0){
                        e =  new_estado(listSize(lista_estados) + estado_actual,
                                         destino,
                                         genera_tipo_lista(afnd, lista_idestados_aux, NORMAL));
                        estado_set_lista_estados(e, lista_idestados_aux);
                        
                        list_pushBack(lista_estados, e);
                        free_estado(e);
                        /*Si no existe liberamos la memoreia de la lista auxiliar*/
                    }else{
                        list_free(lista_idestados_aux);
                    }
                    /*Liberamos memoria*/
                    free(destino);
                    transicion_free(t);
                }

                /*Libero la lista siempre antes de volver a reservar memoria para otra lista*/
                list_free(id_estados);
                
            }
        }

    
    /*Una vez salgo del bucle principal creo el nuevo afd*/
    AFND * afd = new_afd(afnd, lista_estados);

    /*Libero memoria*/
    list_free(lista_estados);

    return afd;
}



/**
 * Funcion que genera el afd pasandole una lista de estados
 * 
 */ 
AFND *new_afd(AFND * afnd, List * le)
{
    int simbolos;
    estado * e;
    transicion *t;
    AFND * afd;

    /*CDE*/
    if(!afnd || !le ){
        return NULL;
    }

    simbolos = AFNDNumSimbolos(afnd);

    /* Reservo memoria para el nuevo afd*/
    afd = AFNDNuevo("determinista", listSize(le), simbolos);

    /* Primero insertamos los simbolos que estan en el afnd
     original ya que estos no deben variar
    */
    for(int i = 0; i < simbolos; i++ ){
        AFNDInsertaSimbolo(afd, AFNDSimboloEn(afnd, i));
    }


    /* Luego inserto todos los estados que tengo en la lista de estados finales */
    for(int i = 0; i < listSize(le); i++ ){
        e =list_getElementInPos(le, i);
        AFNDInsertaEstado(afd, estado_get_nombre(e), estado_get_tipo(e));
    }

    /* Y por ultimo las transiciones ya que si las inserto antes
     no tendria todos los estados destinos de las transiciones
    */
    for(int i = 0; i < listSize(le); i++ ){
        e =list_getElementInPos(le, i);
        for(int j= 0; j<listSize(estado_get_lista_transiciones(e));j ++){
            t = list_getElementInPos(estado_get_lista_transiciones(e), j);
            /* Comprobamos que la transicion exista para insertarla */ 
            if(transicion_get_tipo(t)!= TRANSICION_INEXISTENTE && t != NULL){
             AFNDInsertaTransicion(afd,
                                  estado_get_nombre(e),
                                  transicion_get_simbolo(t),
                                  transicion_get_destino(t));

            }
        }
    }

    return afd;


}


/* Obtengo las transiciones lambda para  un estado unico devuelvo
   una lista con los ids de los estados en el afnd original
*/
List * get_Ltrans(AFND * afnd, int id_estado)
{
  int total_estados;
  int i;
  int estado;
  total_estados = AFNDNumEstados(afnd);
  List * aux;

    aux = list_new(extraint_free, extraint_copy, extraint_print, extraint_cmp);
	
    /* Metemos el estado actual porque siempre podemos "llegar" a él sin consumir nada de al cadena*/
	list_insertInOrder(aux,  &id_estado);


    /* Compruebo todos los estados que hay en el afnd original*/ 
	for ( i = 0; i < total_estados; i++){
        /* Menos el que me dan porque ya lo he tenido en cuenta */
		if (i != id_estado){
            /*Si existe Ltransicion lo pusheo */
			if (AFNDCierreLTransicionIJ(afnd, id_estado, i) == 1){
				list_pushFront(aux, &i);
			}
		}
	}
    return aux;
}


/* Obtengo las transiciones lambda para varios estados devuelvo
   una lista con los ids de los estados en el afnd original
*/
List * get_Ltrans_lista(AFND *afnd, List * le)
{
    List * l;
    List * aux;
    int rep;

    l = list_new(extraint_free, extraint_copy, extraint_print, extraint_cmp);
    for(int i = 0; i<listSize(le); i++){
        aux = get_Ltrans(afnd, *(int *)list_getElementInPos(le, i));

        for(int j = 0; j < listSize(aux); j++ ){
            rep = 0;
            /*Comprobacion de que no este ya el id en la lista*/
            for(int k = 0; k< listSize(l); k++){
                if(*(int *)(list_getElementInPos(aux,  j)) == *(int *)(list_getElementInPos(l, k))){
                    rep = 1;
                }
            }
            if(rep == 0){
                list_insertInOrder(l, list_getElementInPos(aux,  j));
            }
        }
        list_free(aux);
    }
    return l;
}

/* Dado un estado devuelve el nombre que deberia tener dicho estado */
char * genera_nombre_estado(estado * e)
{
    List * aux;
    if(!e){
        return NULL;
    }
    aux = estado_get_lista_estados(e);
    
    return genera_nombre_lista(aux);
}

/* Dado una lista de ids devuelve el nombre que deberia tener un estado con dicha lista */ 
char * genera_nombre_lista(List * l)
{
    int *q;

    if(!l){
        return NULL;
    }
    char * nombre;
    nombre = (char *)calloc(listSize(l) *2 +1, sizeof(char));
    
    for(int i=listSize(l)-1;i>-1;i--){
        q= list_getElementInPos(l, i);
        sprintf(nombre + strlen(nombre), "q%d", *q);
    }

    return nombre;
}

/* Dado un estado devuelve el tipo que deberia tener dicho estado */
int genera_tipo_estado(AFND * afnd, estado * e)
{
    List * aux;
    if(!e){
        return -1;
    }
    aux = estado_get_lista_estados(e);

    return genera_tipo_lista(afnd, aux, estado_get_tipo(e));
}


/* Dado una lista de ids devuelve el tipo que deberia tener un estado con dicha lista */ 
int genera_tipo_lista(AFND * afnd, List *l, int ini)
{
    int tipo_aux;
    int tipo = NORMAL;
    int id_estado;
    if(!l || ini < INICIAL){
        return -1;
    }
    for(int i = 0; i< listSize(l); i++){
        id_estado =*(int *) list_getElementInPos(l,  i);
        tipo_aux = AFNDTipoEstadoEn(afnd, id_estado);
        /*Si obtengo que el estado en el que estoy es inicial y
         es el primer estado que inserto en la lista le asigno inicial
        */
        if(tipo_aux == INICIAL && ini == INICIAL){
         tipo = INICIAL;
        } 
        /* Si obtengo que el estado  en el que estoy es final y
         no es el primer estado de  que se inserta el estado sera un estado final
        */
        if(tipo_aux == FINAL && tipo == NORMAL){
         tipo = FINAL;
        }
        /* Si obtengoq ue llego a un estado final y es el primer
         estado que se inserta el estado sera innicial y final 
        */
        if(tipo_aux == FINAL && ini == INICIAL){
         return INICIAL_Y_FINAL;
        }
        /*si no es ninguna de estas opciones el sestado sera un estado normal */
    }
    return tipo;
}


/* Obtengo las transiciones para un estado unico devuelvo
   una lista con los ids de los estados en el afnd original
*/
List * get_transiciones(AFND * afnd, int estado_actual, int simbolo_pos)
{
    List * l;
    int num_estados;
    int id_estado_actual;
  

    if(!afnd || estado_actual<0 || simbolo_pos < 0){
        return NULL;
    }

    l = list_new(extraint_free, extraint_copy, extraint_print, extraint_cmp);
    if(!l){
        return NULL;
    }

    num_estados = AFNDNumEstados(afnd);
    for(int i = 0; i< num_estados; i++){
        /*  Usado para debuggear
            printf("***********\t%d\t%d\t%d\n",estado_actual, simbolo_pos, i );
        */
        if(AFNDTransicionIndicesEstadoiSimboloEstadof(afnd,estado_actual, simbolo_pos, i)==1){
            list_insertInOrder(l, &i);
        }
    }

        return l;


}


/* Obtengo las transiciones lambda para varios estados devuelvo
   una lista con los ids de los estados en el afnd original
*/
List * get_transiciones_lista(AFND * afnd, List * estados, int simbolo_pos)
{
    int num_simbolos = AFNDNumSimbolos(afnd);
    List *l;
    List * aux;
    int  rep;

    l = list_new(extraint_free, extraint_copy, extraint_print, extraint_cmp);

    for(int i = 0; i < listSize(estados); i++){
        /* Para cada id de la lista obtengo sus transiciones para el simbolo que me pasan*/
        aux = get_transiciones(afnd, *(int *)list_getElementInPos(estados, i), simbolo_pos);

        for(int j = 0; j < listSize(aux); j++ ){
            rep = 0;
            /*Comprobacion de que no este ya el id en la lista*/
            for(int k = 0; k< listSize(l); k++){
                if(*(int *)(list_getElementInPos(aux,  j)) == *(int *)(list_getElementInPos(l, k))){
                    rep = 1;
                }
            }
            /* Si no esta en la lista ya lo pusheo */
            if(rep == 0){
                list_pushBack(l, list_getElementInPos(aux,  j));
            }
        }
        list_free(aux);
    }
    
    return l;
}