#include "pommedapi.h"

Eina_Bool
test_list_filter(
   void *data,
   Eio_File *handler,
   const Eina_File_Direct_Info *info)
{
   if (info->type != EINA_FILE_DIR ) return EINA_FALSE;
   return EINA_TRUE;
}

void
test_list_main(
   void *data,
   Eio_File *handler,
   const Eina_File_Direct_Info *info)
{
   Pommedapi *p = data;
   Pommedapi_Test *pt;

   DBG("p[%p] info->path[%s]", p, info->path);

   pt = test_new(p, info);
   EINA_SAFETY_ON_NULL_RETURN(pt);

   _EINA_LIST_PREPEND(p->tests, pt);
}

void
test_list_done(
   void *data,
   Eio_File *handler)
{
   Pommedapi *p = data;
   DBG("p[%p] : Starting tests", p);

   test_run(p);
}

void
test_list_error(
   void *data,
   Eio_File *handler,
   int error)
{
   Pommedapi *p = data;
   ERR("p[%p]", p);
}
