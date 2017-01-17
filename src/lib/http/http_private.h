#include <Http.h>
#include <Ecore.h>
#include <Ecore_Con.h>

typedef struct _Http_File_Download
{
   const char *file;
   int fd;
} Http_File_Download;

typedef enum _Http_Type
{
   HTTP_FILE_DOWNLOAD,
   HTTP_DATA_DOWNLOAD,
   HTTP_DATA_POST
} Http_Type;

struct _Http
{
   Http_Type type;
   const char *url;
   Ecore_Con_Url *ec;

   struct
   {
      Ecore_Event_Handler *data,
                          *done;
   } ev;

   struct
   {
      Http_Done_Cb done;
      Http_Error_Cb error;
      Http_Progress_Cb progress;
      void *data;
   } cb;

   union
   {
      Http_File_Download hfd;
   } u;
};

extern int _http_log_dom_global;

#define ERR(...) EINA_LOG_DOM_ERR(_http_log_dom_global, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(_http_log_dom_global, __VA_ARGS__)
#define NFO(...) EINA_LOG_DOM_INFO(_http_log_dom_global, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(_http_log_dom_global, __VA_ARGS__)
#define CRI(...) EINA_LOG_DOM_CRIT(_http_log_dom_global, __VA_ARGS__)

void http_free(Http *h);
Http * http_new(const char *url, Eina_List *headers, Http_Done_Cb done_cb, Http_Error_Cb error_cb, Http_Progress_Cb progress_cb, void *data);

Eina_Bool http_events_data(void *data, int type, void *ev);
Eina_Bool http_events_done(void *data, int type, void *ev);
