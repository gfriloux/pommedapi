#include "pommedapi.h"

int
_list_sort(
   const void *d1,
   const void *d2)
{
   Test *t1 = (void *)d1,
        *t2 = (void *)d2;

   if(!t1) return(1);
   if(!t2) return(-1);
   return(strcmp(t1->query.path + t1->query.name_start, t2->query.path + t2->query.name_start));
}

Eina_Bool
list_filter(
   void *data,
   Eio_File *handler,
   const Eina_File_Direct_Info *info)
{
   if (info->type != EINA_FILE_DIR ) return EINA_FALSE;
   return EINA_TRUE;
}

void
list_main(
   void *data,
   Eio_File *handler,
   const Eina_File_Direct_Info *info)
{
   Pommedapi *p = data;
   Test *t;

   DBG("p[%p] info->path[%s]", p, info->path);

   t = test_new(p->conf->host, p->conf->port, info);
   EINA_SAFETY_ON_NULL_RETURN(t);

   _EINA_LIST_PREPEND(p->tests.pending, t);
}

void
list_done(
   void *data,
   Eio_File *handler)
{
   Pommedapi *p = data;
   DBG("p[%p] : Starting %d tests", eina_list_count(p->tests.pending), p);

   p->tests.pending = eina_list_sort(p->tests.pending, eina_list_count(p->tests.pending), _list_sort);

   run_next(p);
}

void
list_error(
   void *data,
   Eio_File *handler,
   int error)
{
   Pommedapi *p = data;
   ERR("p[%p]", p);
}
