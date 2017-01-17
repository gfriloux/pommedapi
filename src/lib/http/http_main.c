#include "http_private.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static int _http_init_count     = 0;
int        _http_log_dom_global = -1;

void
http_header_free(Http_Header *hh)
{
   free((char *)hh->key);
   free((char *)hh->value);
   free(hh);
}

Http_Header *
http_header_new(
   const char *key,
   const char *value)
{
   Http_Header *hh;

   hh = calloc(1, sizeof(Http_Header));
   EINA_SAFETY_ON_NULL_RETURN_VAL(hh, NULL);

   hh->key = strdup(key);
   EINA_SAFETY_ON_NULL_GOTO(hh->key, free_hh);

   hh->value = strdup(value);
   EINA_SAFETY_ON_NULL_GOTO(hh->value, free_hh);

   return hh;

free_hh:
   http_header_free(hh);
   return NULL;
}

int
http_status_code_get(
   Http *http)
{
   return ecore_con_url_status_code_get(http->ec);
}

const Eina_List *
http_headers_get(
   Http *http)
{
   return ecore_con_url_response_headers_get(http->ec);
}

void
http_free(
   Http *h)
{
   DBG("h[%p]", h);
   free( (char*)h->url );

   if (h->ev.data) ecore_event_handler_del(h->ev.data);
   if (h->ev.done) ecore_event_handler_del(h->ev.done);

   switch (h->type)
     {
      case HTTP_FILE_DOWNLOAD:
        {
           if (h->u.hfd.fd)
             {
                close(h->u.hfd.fd);
                h->u.hfd.fd = 0;
             }
           free( (char*)h->u.hfd.file );
           break;
        }
      default:
         break;
     }

   ecore_con_url_free(h->ec);
   free(h);
}

Http *
http_new(
   const char      *url,
   Eina_List       *headers,
   Http_Done_Cb     done_cb,
   Http_Error_Cb    error_cb,
   Http_Progress_Cb progress_cb,
   void            *data)
{
   Http *h;
   Http_Header *hh;
   Eina_List *l;

   h = calloc( 1, sizeof(Http) );
   EINA_SAFETY_ON_NULL_RETURN_VAL(h, NULL);

   h->url = strdup(url);
   EINA_SAFETY_ON_NULL_GOTO(h->url, free_h);

#define _EV(_a, _b, _c, _d)                                                    \
   _a = ecore_event_handler_add(ECORE_CON_EVENT_URL_ ## _b, http_events_ ## _c, _d)

   _EV(h->ev.data, DATA, data, h);
   EINA_SAFETY_ON_NULL_GOTO(h->ev.data, free_h);

   _EV(h->ev.done, COMPLETE, done, h);
   EINA_SAFETY_ON_NULL_GOTO(h->ev.done, free_h);
#undef _EV

   h->cb.done     = done_cb;
   h->cb.error    = error_cb;
   h->cb.progress = progress_cb;
   h->cb.data     = data;

   h->ec = ecore_con_url_new(url);

   EINA_LIST_FOREACH(headers, l, hh)
     ecore_con_url_additional_header_add(h->ec, hh->key, hh->value);

   ecore_con_url_data_set(h->ec, h);
   ecore_con_url_timeout_set(h->ec, 30.0);

   DBG("h[%p] h->ec[%p]", h, h->ec);

   return h;

free_h:
   http_free(h);
   return NULL;
}

Eina_Bool
http_data_post(
   const char      *url,
   Eina_List       *headers,
   Http_Progress_Cb progress_cb,
   Http_Done_Cb     done_cb,
   Http_Error_Cb    error_cb,
   void            *post_data,
   long             post_len,
   const char      *content_type,
   void            *data)
{
   Http *h;

   DBG("url[%s]", url);

   h = http_new(url, headers, done_cb, error_cb, progress_cb, data);
   EINA_SAFETY_ON_NULL_RETURN_VAL(h, EINA_FALSE);

   h->type = HTTP_DATA_POST;
   ecore_con_url_post(h->ec, post_data, post_len, content_type);

   return EINA_TRUE;
}

Eina_Bool
http_data_download(
   const char      *url,
   Eina_List       *headers,
   Http_Progress_Cb progress_cb,
   Http_Done_Cb     done_cb,
   Http_Error_Cb    error_cb,
   void            *data)
{
   Http *h;

   DBG("url[%s]", url);

   h = http_new(url, headers, done_cb, error_cb, progress_cb, data);
   EINA_SAFETY_ON_NULL_RETURN_VAL(h, EINA_FALSE);

   h->type = HTTP_DATA_DOWNLOAD;
   ecore_con_url_get(h->ec);

   return EINA_TRUE;
}

Eina_Bool
http_file_download(
   const char   *url,
   Eina_List    *headers,
   const char   *file,
   Http_Done_Cb  done_cb,
   Http_Error_Cb error_cb,
   void         *data)
{
   Http     *h;
   int       fd;
   Eina_Bool r;

#ifdef _WIN32
   fd = open(file, O_WRONLY | O_CREAT | O_TRUNC);
#else
   fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
#endif
   if (fd == -1)
     {
        ERR( "Failed to open file %s : %s", file, strerror(errno) );
        return EINA_FALSE;
     }

#ifdef _WIN32
   _setmode(fd, _O_BINARY);
#endif

   h = http_new(url, headers, done_cb, error_cb, NULL, data);
   EINA_SAFETY_ON_NULL_GOTO(h, close_fd);

   h->type = HTTP_FILE_DOWNLOAD;

   h->u.hfd.fd = fd;
   ecore_con_url_fd_set(h->ec, h->u.hfd.fd);

   h->u.hfd.file = strdup(file);
   EINA_SAFETY_ON_NULL_GOTO(h->u.hfd.file, free_h);

   r = ecore_con_url_get(h->ec);
   EINA_SAFETY_ON_TRUE_GOTO(!r, free_h);

   return EINA_TRUE;

free_h:
   http_free(h);
close_fd:
   close(fd);
   return EINA_FALSE;
}

int
http_init(
   void)
{
   if (++_http_init_count != 1)
      return _http_init_count;

   if ( !eina_init() )
     {
        printf("http can not initialize Eina\n");
        return --_http_init_count;
     }

   _http_log_dom_global = eina_log_domain_register("http", EINA_COLOR_YELLOW);
   if (_http_log_dom_global < 0)
     {
        EINA_LOG_ERR("http can not create a general log domain.");
        goto shutdown_eina;
     }

   if ( !ecore_init() )
     {
        ERR("http can not initialize Ecore.");
        goto unregister_log_domain;
     }

   if ( !ecore_con_init() )
     {
        ERR("http can not initialize Ecore_Con.");
        goto shutdown_ecore;
     }

   if ( !ecore_con_url_init() )
     {
        ERR("Http can not initialize Ecore_Con_Url.");
        goto shutdown_ecore_con;
     }

   return _http_init_count;

shutdown_ecore_con:
   ecore_con_shutdown();
shutdown_ecore:
   ecore_shutdown();
unregister_log_domain:
   eina_log_domain_unregister(_http_log_dom_global);
   _http_log_dom_global = -1;
shutdown_eina:
   eina_shutdown();
   return --_http_init_count;
}

int
http_shutdown(
   void)
{
   if (_http_init_count <= 0)
     {
        fprintf(stderr, "Http init count not greater than 0 in shutdown.");
        return 0;
     }

   if (--_http_init_count != 0)
      return _http_init_count;

   ecore_con_url_shutdown();
   ecore_con_shutdown();
   ecore_shutdown();
   eina_log_domain_unregister(_http_log_dom_global);
   _http_log_dom_global = -1;
   eina_shutdown();
   return _http_init_count;
}

