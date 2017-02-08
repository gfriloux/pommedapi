#include "pommedapi.h"

void
pommedapi_free(
   Pommedapi *p)
{
   free((char *)p->path.test);
   free((char *)p->path.html);
   free((char *)p->path.conf);
   serialize_free(p->conf, SERIALIZATION_POMMEDAPI_CONF);
   rendering_free(p->rendering);
   free(p);
}

Pommedapi *
pommedapi_new(
   const char *test,
   const char *html,
   const char *filename)
{
   Pommedapi *p;
   size_t l;

   EINA_SAFETY_ON_NULL_RETURN_VAL(test, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(html, NULL);

   l = strlen(test);

   p = calloc(1, sizeof(Pommedapi));
   EINA_SAFETY_ON_NULL_RETURN_VAL(p, NULL);

   p->path.test = strdup(test);
   EINA_SAFETY_ON_NULL_GOTO(p->path.test, free_p);

   p->path.html = strdup(html);
   EINA_SAFETY_ON_NULL_GOTO(p->path.html, free_p);

   p->path.conf = generic_strdupf("%s%spommedapi.conf",
                                  test, test[l] == '/' ? "" : "/");
   EINA_SAFETY_ON_NULL_GOTO(p->path.conf, free_p);

   DBG("p->path.test[%s] p->path.conf[%s]", p->path.test, p->path.conf);

   p->conf = serialize_file_to_struct(p->path.conf, SERIALIZATION_POMMEDAPI_CONF);
   EINA_SAFETY_ON_NULL_GOTO(p->conf, free_p);

   p->rendering = rendering_new(html, filename, p->conf->description);
   EINA_SAFETY_ON_NULL_GOTO(p->rendering, free_p);

   return p;

free_p:
   pommedapi_free(p);
   return NULL;
}
