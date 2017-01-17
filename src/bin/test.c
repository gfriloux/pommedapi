#include "pommedapi.h"

void
test_free(
   Pommedapi_Test *pt)
{
   Http_Header *hh;

   free((char *)pt->path);
   free((char *)pt->query);
   Json_Query_free(pt->conf);
   EINA_LIST_FREE(pt->headers, hh) http_header_free(hh);
   free(pt->post_data.data);
   eina_strbuf_free(pt->result.data);
   free(pt);
}

Pommedapi_Test *
test_new(
   Pommedapi *p,
   const Eina_File_Direct_Info *info)
{
   Pommedapi_Test *pt;
   Json_Query_Header *jqh;
   int ret;
   char *s;
   Eina_List *l;

   pt = calloc(1, sizeof(Pommedapi_Test));
   EINA_SAFETY_ON_NULL_RETURN_VAL(pt, NULL);

   pt->path = strdup(info->path);
   EINA_SAFETY_ON_NULL_GOTO(pt->path, free_pt);

   pt->name_start = info->name_start;

   pt->query = utils_strdupf("%s/query.json", pt->path);
   EINA_SAFETY_ON_NULL_GOTO(pt->query, free_pt);

   pt->conf = serialize_file_to_struct(pt->query, SERIALIZATION_QUERY);
   EINA_SAFETY_ON_NULL_GOTO(pt->conf, free_pt);

   pt->p = p;

   s = utils_strdupf("%s/data", pt->path);
   EINA_SAFETY_ON_NULL_GOTO(s, free_pt);
   ret = access(s, R_OK);
   if (!ret) pt->post_data.data = gfile_data_read(s, &pt->post_data.len);
   free(s);

   EINA_LIST_FOREACH(pt->conf->headers, l, jqh)
     {
        Http_Header *hh;

        hh = http_header_new(jqh->name, jqh->value);
        if (!hh) continue;

        _EINA_LIST_APPEND(pt->headers, hh);
     }

   pt->result.data = eina_strbuf_new();
   EINA_SAFETY_ON_NULL_GOTO(pt->result.data, free_pt);

   DBG("pt->path[%s] pt->conf->uri[%s]", pt->path, pt->conf->uri);

   return pt;

free_pt:
   test_free(pt);
   return NULL;
}

void
test_run(
   Pommedapi *p)
{
   Pommedapi_Test *pt;
   char *s;

   pt = _EINA_LIST_POP(p->tests);
   if (!pt)
     {
        DBG("Over!");
        ecore_main_loop_quit();
        return;
     }

   DBG("Running test [%s]", pt->path + pt->name_start);

   s = utils_strdupf("http://%s:%d%s", p->conf->host, p->conf->port, pt->conf->uri);

   gettimeofday(&pt->perf.start, 0);

   if (pt->post_data.data)
     {
        http_data_post(s, pt->headers, test_query_progress, test_query_done,
                       test_query_error, pt->post_data.data, pt->post_data.len,
                       "application/x-www-form-urlencoded; charset=UTF-8", pt);
     }
   else
     http_data_download(s, pt->headers, test_query_progress,
                        test_query_done, test_query_error, pt);
   free(s);
}
