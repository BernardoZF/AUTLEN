#include "estado.h"
#include "afnd.h"
#include "transicion.h"
#include "list.h"
#include "extrafunctions_int.h"
#include <string.h>


 struct _estado{
    int id;                            /* Id del estado */
    char *nombre;                      /* Nombre del estado */
    int tipo;                          /* tipo de estado (ver tipos en afnd.h)*/
    List * estados_componentes;        /* Lista de ids de los estados que componen a este estado en el afnd original */
    List * transiciones;               /* Lista de transiciones que salen del estado*/
};


estado * new_estado(int id, char* nombre, int tipo)
{
    estado *e;

    if(id < 0 || nombre == NULL ){
       return NULL;
    }
    if(tipo != INICIAL && tipo != FINAL && tipo != INICIAL_Y_FINAL && tipo != NORMAL){
       return NULL;
    }

    e=(estado *)malloc(sizeof(estado));
   if(!e){
      return NULL;
   }

   e->id = id;
   e->nombre = (char *)calloc(strlen(nombre) +1 , sizeof(char));
   strcpy(e->nombre, nombre);
   e->tipo = tipo;
   e->estados_componentes = list_new(extraint_free, extraint_copy, extraint_print, extraint_cmp);
   e->transiciones = list_new(transicion_free,transicion_copy,transicion_print,transicion_cmp);

   return e;
}

void free_estado(void * e)
{
   if(!e){
      return;
   }
   free(estado_get_nombre(e));
   if(estado_get_lista_estados(e))
   list_free(estado_get_lista_estados(e));
   if(estado_get_lista_transiciones(e))
   list_free(estado_get_lista_transiciones(e));
   free(e);
}
 
void * estado_cpy(const void * src)
{
   estado * e;
   e = new_estado(estado_get_id(src), estado_get_nombre(src), estado_get_tipo(src));
   if(!e){
      return NULL;
   }
   if(estado_get_lista_estados(src) != NULL){
      for(int i= 0; i<listSize(estado_get_lista_estados(src)); i++){
         list_insertInOrder(e->estados_componentes, list_getElementInPos(estado_get_lista_estados(src),i));
      }
   }
   
   for(int i= 0; i<listSize(estado_get_lista_transiciones(src)); i++){
      list_pushBack(e->transiciones, list_getElementInPos(((estado *)src)->transiciones,i));
   }
   return (void *)e;
}

int estado_get_id(const estado * e)
{
   if(!e){
      return -1;
   }

   return e->id;
}

char * estado_get_nombre(const estado * e)
{
   if(!e){
      return NULL;
   }

   return e->nombre;
}

int estado_get_tipo(const estado* e)
{
   if(!e){
      return -1;
   }

   return e->tipo;
}

List * estado_get_lista_estados(const estado * e)
{
   if(!e){
      return NULL;
   }

   return e->estados_componentes;
}

List * estado_get_lista_transiciones(const estado * e)
{
   if(!e){
      return NULL;
   }

   return e->transiciones;
}

void estado_set_id(estado * e, int id)
{
   if(!e || id<0){
      return;
   }

   e->id = id;
}

void estado_set_nombre(estado * e, char * nombre)
{
   if(!e || !nombre){
      return ;
   }
   
  e->nombre = realloc( e->nombre, (strlen(nombre) + 1) *sizeof(char));
  strcpy(e->nombre,nombre);
}

void estado_set_tipo(estado* e, int tipo)
{
   if(!e){
      return;
   }
   if(tipo != INICIAL && tipo != NORMAL && tipo != FINAL && tipo != INICIAL_Y_FINAL){
      return;
   }

   e->tipo = tipo;
}

void estado_set_lista_estados(estado * e, List * l)
{
   if(!e || !l){
      return;
   }
   if(e->estados_componentes){
      list_free(e->estados_componentes);
   }
   e->estados_componentes = l;

}

int estado_cmp(const void * e1, const void * e2)
{
   if(!e1 || !e2){
      return -2;
   }

   if(estado_get_id(e1) < estado_get_id(e2)){
      return -1;
   }
   else if(estado_get_id(e1) == estado_get_id(e2)){
      return 0;
   }
   else{
      return 1;
   }
}

/*
* Sin implementar porque no la necesito
*/
int estado_print(FILE * pf, const void * e)
{
   return 1;
}

Status estado_lista_combinaciones_push(estado * e, int id)
{
   if(!e || id<0){
      return ERROR;
   }

   return list_insertInOrder(e->estados_componentes, &id);
}

Status estado_lista_transiciones_push(estado * e, transicion * t)
{
   if(!e || !t){
      return ERROR; 
   }
   if(transicion_get_destino(t))

   /*  Usado para debuggear
      printf("\n**************************************************%s\n", transicion_get_destino(t));
   */
   return list_pushBack(e->transiciones, t);
}