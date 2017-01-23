#include "test_private.h"

#include <unistd.h>

static int _test_init_count     = 0;
int        _test_log_dom_global = -1;

Eina_Bool
test_run(
   Test *t,
   Test_Done_Cb done,
   Test_Error_Cb error,
   void *data)
{
   Test_Run *tr;

   DBG("Running test [%s]", t->query.path + t->query.name_start);

   tr = test_run_new(t, done, error, data);
   EINA_SAFETY_ON_NULL_RETURN_VAL(tr, EINA_FALSE);

   gettimeofday(&t->query.time, 0);

   if (t->query.data.data)
     http_data_post(t->query.url, t->query.headers,
                    test_run_progress, test_run_done, test_run_error,
                    t->query.data.data, t->query.data.len,
                    "application/x-www-form-urlencoded; charset=UTF-8", tr);
   else
     http_data_download(t->query.url, t->query.headers,
                        test_run_progress, test_run_done, test_run_error, tr);
}

void
test_free(
   Test *t)
{
   Http_Header *hh;

   serialize_free(t->conf, SERIALIZATION_QUERY);

   free((char *)t->query.path);
   free((char *)t->query.file);
   free((char *)t->query.url);
   free((char *)t->query.data.data);
   EINA_LIST_FREE(t->query.headers, hh) http_header_free(hh);

   eina_strbuf_free(t->result.data.buf);

   free(t);
}

Test *
test_new(
   const char *host,
   unsigned int port,
   const Eina_File_Direct_Info *info)
{
   Test *t;
   char *s;
   int ret;
   Json_Query_Header *jqh;
   Eina_List *l;

   t = calloc(1, sizeof(Test));
   EINA_SAFETY_ON_NULL_RETURN_VAL(t, NULL);

   t->query.path = strdup(info->path);
   EINA_SAFETY_ON_NULL_GOTO(t->query.path, free_t);

   t->query.name_start = info->name_start;

   t->query.file = test_utils_strdupf("%s/query.json", t->query.path);
   EINA_SAFETY_ON_NULL_GOTO(t->query.file, free_t);

   t->conf = serialize_file_to_struct(t->query.file, SERIALIZATION_QUERY);
   EINA_SAFETY_ON_NULL_GOTO(t->conf, free_t);

   t->query.url = test_utils_strdupf("http://%s:%d%s", host, port, t->conf->uri);
   EINA_SAFETY_ON_NULL_GOTO(t->query.url, free_t);

   s = test_utils_strdupf("%s/data", t->query.path);
   EINA_SAFETY_ON_NULL_GOTO(s, free_t);
   ret = access(s, R_OK);
   if (!ret) t->query.data.data = gfile_data_read(s, &t->query.data.len);
   free(s);

   EINA_LIST_FOREACH(t->conf->headers, l, jqh)
     {
        Http_Header *hh;

        hh = http_header_new(jqh->name, jqh->value);
        if (!hh) continue;

        _EINA_LIST_APPEND(t->query.headers, hh);
     }

   t->result.data.buf = eina_strbuf_new();
   EINA_SAFETY_ON_NULL_GOTO(t->result.data.buf, free_t);

   DBG("t->query.path[%s] t->conf->uri[%s]", t->query.path, t->conf->uri);

   return t;

free_t:
   test_free(t);
   return NULL;
}

int
test_init(
   void)
{
   if (++_test_init_count != 1)
     return _test_init_count;

   if (!eina_init())
     {
        fprintf(stderr, "Test can not initialize Eina\n");
        return --_test_init_count;
     }

   _test_log_dom_global = eina_log_domain_register("test", EINA_COLOR_YELLOW);
   if (_test_log_dom_global < 0)
     {
        EINA_LOG_ERR("Test can not create a general log domain.");
        goto shutdown_eina;
     }

   return _test_init_count;

shutdown_eina:
   eina_shutdown();
   return --_test_init_count;
}

int
test_shutdown(
   void)
{
   if (_test_init_count <= 0)
     {
        fprintf(stderr, "Test init count not greater than 0 in shutdown.");
        return 0;
     }

   if (--_test_init_count != 0)
     return _test_init_count;

   eina_log_domain_unregister(_test_log_dom_global);
   _test_log_dom_global = -1;

   eina_shutdown();
   return _test_init_count;
}
