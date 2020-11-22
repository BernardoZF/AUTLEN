#include "extrafunctions_int.h"


/**
* Estas funciones las hemos creado debido a que las otras que estan en libstack.a
* no imprimen con espacios entre los enteros lo que hace imposible la comprension
* de la impresion de las listas.
*/
int extraint_print(FILE *pf, const void  *a)
{
  int ret = 0;

  if (!pf || !a) return -1;

  ret += fprintf(pf, "%d  ",  *(int *)a);

return ret;
}


void * extraint_copy (const void* a)
{
  int *cpy=NULL;

  if(!a) return  NULL;

  cpy=(int *)malloc(sizeof(int));
  if(!cpy)
    return NULL;

  *cpy = *(int  *)a;

  return  (void *)cpy;
}

void extraint_free(void * a)
{
  if(!a)
  return;

  free(a);
}


int extraint_cmp(const void * a, const void * b)
{

  if(!a || !b) return -2;

    if(*(int *)a < *(int *)b)
      return -1;
    else if(*(int *)a == *(int *) b)
      return 0;
    else
      return 1;
}
