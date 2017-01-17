#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <Eina.h>
#include <Ecore.h>
#include <Ecore_Getopt.h>
#include <Eio.h>
#include <Gfile.h>
#include <Serialize.h>
#include <Http.h>

typedef struct _Pommedapi
{
   Json_Pommedapi_Conf *conf;

   struct
   {
      char *testdir,
           *conf;
   } path;

   Eina_List *tests;
} Pommedapi;

typedef struct _Pommedapi_Test
{
   Pommedapi *p;
   const char *path,
              *query;
   unsigned int name_start;

   struct
   {
      char *data;
      size_t len;
   } post_data;

   struct
   {
      struct timeval start,
                     stop;
      float latency;
   } perf;

   struct
   {
      int code;
      Eina_Strbuf *data;
   } result;

   Json_Query *conf;
   Eina_List *headers;
} Pommedapi_Test;

#define _EINA_LIST_APPEND(_a, _b)  _a = eina_list_append(_a, _b)
#define _EINA_LIST_PREPEND(_a, _b) _a = eina_list_prepend(_a, _b)
#define _EINA_LIST_REMOVE(_a, _b)  _a = eina_list_remove(_a, _b)
#define _EINA_LIST_POP(_a)         _eina_list_pop(&_a)

extern int _pommedapi_log_dom_global;

#define ERR(...) EINA_LOG_DOM_ERR (_pommedapi_log_dom_global, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG (_pommedapi_log_dom_global, __VA_ARGS__)
#define NFO(...) EINA_LOG_DOM_INFO(_pommedapi_log_dom_global, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(_pommedapi_log_dom_global, __VA_ARGS__)
#define CRI(...) EINA_LOG_DOM_CRIT(_pommedapi_log_dom_global, __VA_ARGS__)

static inline void *
_eina_list_pop(Eina_List **l)
{
   void *data;

   data = eina_list_nth(*l, 0);
   _EINA_LIST_REMOVE(*l, data);
   return data;
}

Pommedapi * pommedapi_new(const char *testdir);
void pommedapi_free(Pommedapi *p);

Pommedapi_Test * test_new(Pommedapi *p, const Eina_File_Direct_Info *info);
void test_free(Pommedapi_Test *pt);
void test_run(Pommedapi *p);

Eina_Bool test_list_filter(void *data, Eio_File *handler, const Eina_File_Direct_Info *info);
void test_list_main(void *data, Eio_File *handler, const Eina_File_Direct_Info *info);
void test_list_done(void *data, Eio_File *handler);
void test_list_error(void *data, Eio_File *handler, int error);

void test_query_progress(void *data, char *url_data, size_t url_size);
void test_query_done(void *data, Http *http);
void test_query_error(void *data, const char *error);

char * utils_strdupf(const char *s, ...);
float utils_difftime(struct timeval start, struct timeval end);
