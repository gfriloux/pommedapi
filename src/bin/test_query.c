#include "pommedapi.h"

void
test_query_progress(
   void *data,
   char *url_data,
   size_t url_size)
{
   Pommedapi_Test *pt = data;

   DBG("url_data[%p] url_size[%zu]", url_data, url_size);
   eina_strbuf_append_length(pt->result.data, url_data, url_size);
}

void
test_query_done(
   void *data,
   Http *http)
{
   Pommedapi_Test *pt = data;
   int code;

   gettimeofday(&pt->perf.stop, 0);

   pt->perf.latency = utils_difftime(pt->perf.start, pt->perf.stop);

   pt->result.code = http_status_code_get(http);

   DBG("pt[%p] : Got HTTP code %d and %zu bytes of data in %.02fms",
       pt, pt->result.code, eina_strbuf_length_get(pt->result.data), pt->perf.latency);

   test_run(pt->p);

   test_free(pt);
}

void
test_query_error(
   void *data,
   const char *error)
{
   Pommedapi_Test *pt = data;

   ERR("pt[%p] : %s", pt, error);
}
