#include "test_private.h"

void
test_run_free(
   Test_Run *tr)
{
   free(tr);
}

Test_Run *
test_run_new(
   Test *t,
   Test_Done_Cb done,
   Test_Error_Cb error,
   void *data)
{
   Test_Run *tr;

   tr = calloc(1, sizeof(Test_Run));
   EINA_SAFETY_ON_NULL_RETURN_VAL(tr, NULL);

   tr->t        = t;
   tr->cb.done  = done;
   tr->cb.error = error;
   tr->cb.data  = data;

   return tr;
}

void
test_run_progress(
   void  *data,
   char  *url_data,
   size_t url_size)
{
   Test_Run *tr = data;

   DBG("url_data[%p] url_size[%zu]", url_data, url_size);
   eina_strbuf_append_length(tr->t->result.data.buf, url_data, url_size);
}

void
test_run_done(
   void *data,
   Http *http)
{
   Test_Run *tr = data;

   gettimeofday(&tr->t->result.time, 0);

   tr->t->result.latency = test_utils_difftime(tr->t->query.time, tr->t->result.time);

   tr->t->result.code = http_status_code_get(http);

   DBG("t[%p] : Got HTTP code %d and %zu bytes of data in %.02fms",
       tr->t, tr->t->result.code, eina_strbuf_length_get(tr->t->result.data.buf), tr->t->result.latency);

   tr->cb.done(tr->cb.data, tr->t);
   test_run_free(tr);
}

void
test_run_error(
   void *data,
   const char *error)
{
   Test_Run *tr = data;

   tr->cb.error(tr->cb.data, tr->t, error);
   test_run_free(tr);
}
