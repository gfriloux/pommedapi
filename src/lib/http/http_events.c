#include "http_private.h"

Eina_Bool
http_events_data(
   void *data,
   int   type EINA_UNUSED,
   void *ev)
{
   Http                     *h        = data;
   Ecore_Con_Event_Url_Data *url_data = ev;

   if (h->ec != url_data->url_con) return EINA_TRUE;

   DBG("h[%p]", h);

   if (h->type == HTTP_FILE_DOWNLOAD) return EINA_TRUE;

   h->cb.progress(h->cb.data, (char*)url_data->data, url_data->size);
   return EINA_TRUE;
}

Eina_Bool
http_events_done(
   void *data,
   int   type EINA_UNUSED,
   void *ev)
{
   Http                         *h            = data;
   Ecore_Con_Event_Url_Complete *url_complete = ev;
   int                           status;

   if (h->ec != url_complete->url_con) return EINA_TRUE;

   DBG("h[%p]", h);

   status = ecore_con_url_status_code_get(h->ec);
   if (!status) h->cb.error(h->cb.data, "Failed to query web server");
   else h->cb.done(h->cb.data, h);
   http_free(h);
   return EINA_TRUE;
}

