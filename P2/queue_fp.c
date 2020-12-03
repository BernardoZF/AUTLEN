#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "queue_fp.h"
#include "list.h"

/* ----------- queue.c -------------- */
struct _Queue {
    List * l;
};
/********************PRIVATE FUNCTIONS************************/
Bool queue_isFull(const Queue *q);
/*************************************************************/

Queue *queue_init (free_element_function_type fn_d,
copy_element_function_type fn_c,
print_element_function_type fn_p,
cmp_element_function_type fn_cmp)
{
    Queue *q = NULL;
    if (!fn_d || !fn_c || !fn_p) return NULL;
    q =(Queue *) malloc (sizeof (Queue));
    if (!q) {
        fprintf(stderr, "%s", strerror(errno));
        return NULL;
    }

    /*Assign pointer functions*/
    /*
    q->fc = fn_c;
    q->fd = fn_d;
    q->fp = fn_p;*/

    /*Assign array pointers*/
   /* for (i=0; i < MAXQUEUE; i++)
        q->items[i] = NULL;

    q->front = q->rear = 0;*/
    q->l = list_new(fn_d,fn_c, fn_p, fn_cmp);
    if(!q->l){
        free(q);
        return NULL;
    }


    return q;
}



void queue_free(Queue *q)
{


    if(!q)
        return;
        /*
    while (queue_isEmpty(q)!= TRUE){
        aux = queue_extract(q);
        q->fd(aux);
    }*/
    if(q->l){
        list_free(q->l);
    }
    free(q);

    return;
}



Bool queue_isEmpty(const Queue *q)
{
    if(!q)
        return FALSE;
    /*if(q->front == q->rear){
        return TRUE;
    }*/
    
    return list_Empty(q->l);
}



Status queue_insert(Queue *q, const void* pElem)
{
    if(!q || !pElem)
        return ERROR;

    if (queue_isFull(q) == TRUE){
        fprintf(stderr, "ERROR, COLA LLENA");
        return ERROR;
    }

   /* q->items[q->rear] = q->fc(pElem);
        if(q->items[q->rear]==NULL){
            fprintf(stderr, "ERROR AL COPIAR EL ELEMENTO");
            return ERROR;
        }
    q->rear = (q->rear + 1) % MAXQUEUE;*/
    

    return list_pushBack(q->l, pElem);
}



void * queue_extract(Queue *q){

    if(!q)
        return NULL;
    if(queue_isEmpty(q) == TRUE){
        fprintf(stderr, "ERROR, COLA VACIA");
        return NULL;
    }
    /*
    aux = q->items[q->front];
    q->items[q->front]=NULL;

    q->front = (q->front + 1) % MAXQUEUE;*/


    return list_popFront(q->l);
}


int queue_size (const Queue *q)
{
    if(!q)
        return -1;
    /*if(q->rear >= q->front ){
      return (q->rear - q->front);
    }*/

    return listSize(q->l);
}


int queue_print(FILE *pf, const Queue *q)
{
    if(!pf || !q){
        fprintf(stderr, "ERROR EN LA IMPRESION\n");
        return -1;
    }

    if(queue_isEmpty(q)==TRUE){
      fprintf(stderr, "ERROR, COLA VACIA\n");
      return -1;
    }
    /*
    tam=queue_size(q);
    for(i=0;i<tam;i++){
        ret+=q->fp(pf, q->items[i]);
    }*/

    return listPrint(pf, q->l);
}


Bool  queue_isFull(const Queue *q){
      return FALSE;
}