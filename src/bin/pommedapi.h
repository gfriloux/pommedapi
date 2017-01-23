#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <Eina.h>
#include <Ecore.h>
#include <Ecore_Getopt.h>
#include <Eio.h>
#include <Serialize.h>
#include <Http.h>
#include <Test.h>
#include <Generic.h>
#include <Rendering.h>

typedef struct _Pommedapi
{
   Json_Pommedapi_Conf *conf;
   Rendering *rendering;

   struct
   {
      char *test,
           *html,
           *conf;
   } path;

   Test *current;

   struct
   {
      Eina_List *pending,
                *render;
   } tests;
} Pommedapi;

extern int _pommedapi_log_dom_global;

#define ERR(...) EINA_LOG_DOM_ERR (_pommedapi_log_dom_global, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG (_pommedapi_log_dom_global, __VA_ARGS__)
#define NFO(...) EINA_LOG_DOM_INFO(_pommedapi_log_dom_global, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(_pommedapi_log_dom_global, __VA_ARGS__)
#define CRI(...) EINA_LOG_DOM_CRIT(_pommedapi_log_dom_global, __VA_ARGS__)

Pommedapi * pommedapi_new(const char *test, const char *html);
void pommedapi_free(Pommedapi *p);

Eina_Bool list_filter(void *data, Eio_File *handler, const Eina_File_Direct_Info *info);
void list_main(void *data, Eio_File *handler, const Eina_File_Direct_Info *info);
void list_done(void *data, Eio_File *handler);
void list_error(void *data, Eio_File *handler, int error);

void run_next(Pommedapi *p);

char * utils_strdupf(const char *s, ...);
