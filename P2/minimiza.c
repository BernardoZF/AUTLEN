#include "minimiza.h"
#include "afnd.h"
#include "list.h"
#include <stdio.h>
#include <string.h>
#include "extrafunctions_int.h"
#include "types.h"
#include "clases_de_estados.h"
#include "queue_fp.h"



AFND * AFNDMinimiza(AFND * afd)
{
    AFND * afd_min;
    List * lista_clases = NULL;
    List * accesibles;
    int n_clases;
    int check;
    int cont;
    clase * e = NULL;

    
    
    /* simplifico los estados inaccesibles busqueda en anchura los que no se pueda acceder se eliminan*/
    accesibles = prog2_breadthFirst_adapted(afd);
    if(!accesibles){
        return NULL;
    }
    /*  Print usado para debugear 
        listPrint(stdout, accesibles);
    */


    /* Divido los estados en 2 clases finales y no finales */
    lista_clases = division_inicial(afd, accesibles);
    if(lista_clases == NULL) {
        return NULL;
    }
    /* Una vez ya tengo divididos en clases los estados accesibles ya no necesito esta lista */
    list_free(accesibles);


   /* Bucle principal */
  do{
        /* Obtengo el numero de clases antes de que se dividan */
        n_clases =  listSize(lista_clases);

        /*  Recorremos la lista de clases hasta alcanzar el n_clases antes de que se dividan
            (ya que estamos insertando estados al ddividir no puedo ponerlo hasta listSize porque seria un bucle infinito)
        */
        for(int clase_actual = 0; clase_actual < n_clases; clase_actual ++){
           dividir_clase(afd, clase_actual, lista_clases);
        }
        /*  Borrado de estados redundantes ya que estamos insertando las nuevas 
            "subclases" al final de la lista
        */
        for(int i = 0; i < n_clases; i++){
            e = list_popFront(lista_clases);
            clase_free(e);
                
                
        }
   }while(n_clases != listSize(lista_clases));
   /*  si el numero de clases no cambia hemos terminado de dividir las clases.*/
   
   /* creamos el nuevo afd minimizado */ 
   afd_min = crate_min_afd(afd, lista_clases);

   /*liberar memoria*/
   list_free(lista_clases);

   return afd_min;
}

AFND * crate_min_afd(AFND * afd, List * lc)
{
    AFND * afd_min;
    char * nombre;
    List * aux = NULL;
    if(!afd || !lc){
        return NULL;
    }
    

    afd_min = AFNDNuevo("determinista", listSize(lc), AFNDNumSimbolos(afd));
    if(!afd_min){
        return NULL;
    }
    
    for(int i = 0;i<AFNDNumSimbolos(afd); i++){
        AFNDInsertaSimbolo(afd_min, AFNDSimboloEn(afd, i));
    }

    for(int i = 0 ; i<listSize(lc); i++){
        nombre = genera_nombre_lista(clase_get_lista(list_getElementInPos(lc, i))); // lo mismo hay que liberar memoria de esto
        if(nombre != NULL){
            AFNDInsertaEstado(afd_min, nombre, clase_get_tipo(list_getElementInPos(lc, i)));
            free(nombre);
            nombre = NULL;  
        }
    }

    for(int estado_actual = 0 ; estado_actual< listSize(lc); estado_actual++ ){
        for(int simbolo = 0; simbolo < AFNDNumSimbolos(afd); simbolo++){
            aux = get_transiciones_lista(afd, clase_get_lista(list_getElementInPos(lc, estado_actual)), simbolo);
            if(listSize(aux) > 0){
                char * destino = genera_nombre_destino_clases(lc, aux);
                printf("%s\n", destino );
                nombre = genera_nombre_lista(clase_get_lista(list_getElementInPos(lc, estado_actual)));
                AFNDInsertaTransicion(afd_min, nombre, AFNDSimboloEn(afd, simbolo), destino);
                free(nombre);
                nombre = NULL;
                free(destino);
                destino = NULL;
                list_free(aux);
            }
            
        }
    }

    return afd_min;
}

void dividir_clase(AFND * afd, int clase_actual, List * lista_clases)
{
    int num_estados ;
    int  flag;
    int bandera;
    int estado_id;
    int id_aux;
    List * lista_chequeados = NULL;
    clase * aux;
    
    if(!afd || !lista_clases || clase_actual < 0){
        return;
    }
    
    if(!clase_get_lista(list_getElementInPos(lista_clases, clase_actual))){
        return;
    }
    num_estados = listSize(clase_get_lista(list_getElementInPos(lista_clases, clase_actual)));
    lista_chequeados = list_new(extraint_free, extraint_copy, extraint_print, extraint_cmp);
    if(lista_chequeados == NULL) {
        return;
    }

    printf("%d\n", num_estados);

    for(int estado_actual = 0 ; estado_actual< num_estados; estado_actual++){
            estado_id = *(int *)list_getElementInPos(clase_get_lista(list_getElementInPos(lista_clases, clase_actual)), estado_actual);
           /* compruebo que no lo he cheackeado ya */
           flag = 0;
           for(int i = 0; i< listSize(lista_chequeados) && flag == 0;i++){
               if (estado_id == *(int *)list_getElementInPos(lista_chequeados, i)){
                   flag = 1;
               }
           }
            
           if(flag == 0){
               list_pushBack(lista_chequeados, &estado_id);
                aux = clase_ini(estado_id, AFNDTipoEstadoEn(afd, estado_id));
                for(int j = estado_actual +1; j < num_estados; j++){
                    id_aux = *(int *)list_getElementInPos(clase_get_lista(list_getElementInPos(lista_clases, clase_actual)), j);
                    /* compruebo que no lo he cheackeado ya */
                    bandera = 0;
                    for(int i = 0; i< listSize(lista_chequeados) && bandera == 0;i++){
                        if (id_aux == *(int *)list_getElementInPos(lista_chequeados, i)){
                            bandera = 1;
                        }
                    }
                    if(bandera == 0){
                        /*compruebo equivalencia*/
                        if(equivalentes(afd, estado_id, id_aux, lista_clases) == TRUE){
                            clase_inserta(afd, aux, id_aux);
                            /* j pasa a estar checkeado */
                            list_pushBack(lista_chequeados, &id_aux);
                        }
                        
                    }
                }
             list_pushBack(lista_clases, aux);
             clase_free(aux);
            } 
    }
    list_free(lista_chequeados);
    /*list_destroy_element_inpos(lista_clases , clase_actual);*/

    
}

List * division_inicial(AFND * afd, List *l)
{
    List * lista_clases = NULL;
    clase * clase_final = NULL;
    clase * clase_normal = NULL;
    int tipo = -1;
    int aux;

    if(!afd ||!l){
        return NULL;
    }

    lista_clases = list_new(clase_free, clase_cpy, clase_print, clase_cmp);
    if(!lista_clases){
        return NULL;
    }

    for(int i = 0; i < listSize(l); i++ ){
        aux =*(int *) list_getElementInPos(l, i);
         tipo = AFNDTipoEstadoEn(afd, aux);
         if(tipo == FINAL || tipo == INICIAL_Y_FINAL){
             if(clase_final == NULL){
                    clase_final = clase_ini(aux, tipo);
                    printf("primera inserccion de los finales\n");
                }else{
                    clase_inserta(afd, clase_final, aux);
                    printf("siguientes insercciones exitosas\n");
                }
         }else{
             if(clase_normal == NULL){
                 clase_normal = clase_ini(aux, tipo);
                 printf("primera inserccion de los NO finales\n");
             }else{
                 clase_inserta(afd, clase_normal, aux);
                 printf("insercciones  normales hechas\n");
             }  
         }
    }
    

    list_pushBack(lista_clases, clase_normal);
    list_pushBack(lista_clases, clase_final);
    clase_free(clase_final);
    clase_free(clase_normal);

    return lista_clases;
}

Bool equivalentes(AFND * afd, int a, int b, List *lista_clases)
{
    int x = -3;
    int y = -2;
    for(int simbolo_actual = 0; simbolo_actual < AFNDNumSimbolos(afd); simbolo_actual++){
        x = get_clase_transicion(afd, a, simbolo_actual, lista_clases);
        y = get_clase_transicion(afd, b, simbolo_actual, lista_clases);
        if(x != y){
            return FALSE;
        }
    }
    
    return TRUE;
}

int  get_clase_transicion(AFND * afd, int estado, int simbolo_actual, List *lista_clases)
{
    int destino = -1;
    int num_estados = AFNDNumEstados(afd);

    for(int i = 0; i< num_estados && destino == -1; i++){
        if(AFNDTransicionIndicesEstadoiSimboloEstadof(afd, estado, simbolo_actual, i)==1){
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
    /*int ret = 0;*/
    List * aux = NULL;
    for(int i = 0; i < listSize(lista_clases) ; i++){
        aux = clase_get_lista(list_getElementInPos(lista_clases, i));
        for(int j = 0; j< listSize(aux); j++){
            if(*(int *)list_getElementInPos(aux, j) == destino){
               return i;
            }
        }
    }
    return -1;
}

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
                list_insertInOrder(l, list_getElementInPos(aux,  j));
            }
        }
        list_free(aux);
    }
    
    return l;
}

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

char * genera_nombre_lista(List * l)
{
    int *q;
    char * nombre = NULL;

    if(!l){
        return NULL;
    }
    if(listSize(l) < 1){
        return NULL;
    }
    nombre = malloc((listSize(l) * MAX_STATE_NAME +1)*4);

    q= list_getElementInPos(l, 0);
    sprintf(nombre , "q%d", *q);
    
    for(int i=1;i<listSize(l);i++){
        q= list_getElementInPos(l, i);
        sprintf(nombre + strlen(nombre) , "q%d", *q);
    }

    return nombre;
}

char * genera_nombre_destino_clases(List * lc, List * id_destinos)
{
    int id_ejemplo;
    int clase;
    if(!lc || !id_destinos){
        return NULL;
    }
    id_ejemplo = *(int *)list_getElementInPos(id_destinos, 0);
    clase = get_clase(id_ejemplo, lc);

    return genera_nombre_lista(clase_get_lista(list_getElementInPos(lc, clase)));

}

List * prog2_breadthFirst_adapted (AFND *afd)
{
    Queue * q = NULL;
    List * accesibles;
    List * laux;
    int  * id;
    int ini;
    int aux;
    int num_simbolos;
    int flag = ERROR;
    int checked = FALSE;

    if(!afd ){
      return NULL;
    }

    /* Inicializacion de la cola para la busqueda en anchura y correspondiente  CDE*/
    q=queue_init(extraint_free, extraint_copy, extraint_print, extraint_cmp);
    if(!q){
        return NULL;
    }

    /* Inicializacion de la lista para guardar los estados accesibles y correspondiente  CDE*/
    accesibles = list_new(extraint_free, extraint_copy, extraint_print, extraint_cmp);
    if(!accesibles){
      queue_free(q);
      return NULL;
    }
    
    /*  Primera inserccion en este caso no hace falta definir el inicio de la busqueda
        de la busqueda ya que siempre sera el estado inicial
    */
    ini = AFNDIndiceEstadoInicial(afd);
    flag =queue_insert(q, &ini);

    num_simbolos = AFNDNumSimbolos(afd);

    /*  Print usado para debuggear
        printf("numero de simbolos %d\n", num_simbolos);
    */

   /*   Bucle principal de la busqueda en anchura */
    while(queue_isEmpty(q) == FALSE ){
        id=(int *)queue_extract(q);
        if(!id){
            queue_free(q);
            return NULL;
        }

        /*  Print usado para debuggear
            printf("estado comprobando sus conexiones %d\n", *id);
        */

    
        /*  ahora compruebo para cada estado accesible todas las conexiones con cada simbolo */
        for(int i = 0; i < num_simbolos; i++){
            laux = get_transiciones(afd, *id, i);

            /*  Prints usado para debuggear
                printf(" conexiones del estado %d \n", *id);
                listPrint(stdout, laux);
            */
           /* CDE */
            if(!laux){
                list_free(accesibles);
                queue_free(q);
                return NULL;
            }

            /*  por cada conexion con el simbolo i (al ser un afd se que solo sera  solo 1 pero
                lo sigo usando con lista porque esto se tiene en cuenta con las condiciones del bucle)
            */
            for(int j=0;j<listSize(laux);j++){
                aux=*(int *)list_getElementInPos(laux, j);
                if(aux < 0){
                    queue_free(q);
                    free(id);
                    list_free(accesibles);
                    return NULL;
                }
                checked = FALSE;
                /* Compruebo que no esta ya en la lista */
                for (int k = 0; k < listSize(accesibles) && checked == FALSE; k++){
                    if(aux == *(int *)list_getElementInPos(accesibles, k)){
                        checked = TRUE;
                    }
                }
                /* Si no esta en la lista lo inserto en la cola de estados a revisar */
                if(checked == FALSE){
                    if(queue_insert(q, &aux) == ERROR){
                        queue_free(q);
                        free(id);
                        list_free(accesibles);
                        return NULL;
                    }
                    /*  Inserto el estado que he obtenido en accesibles, 
                        ya que se que he podido acceder a este estado desde el estado id
                        y no esta ya anyadido
                    */
                    list_pushBack(accesibles, &aux);
                        
                }
            }
            
            
            list_free(laux);
        }
        free(id);
        
    }
    queue_free(q);

    return accesibles;
}


