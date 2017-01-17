#include "pommedapi.h"

void
pommedapi_free(Pommedapi *p)
{
   free((char *)p->path.testdir);
   free((char *)p->path.conf);
   serialize_free(p->conf, SERIALIZATION_POMMEDAPI_CONF);
   free(p);
}

Pommedapi *
pommedapi_new(const char *testdir)
{
   Pommedapi *p;
   size_t l;

   EINA_SAFETY_ON_NULL_RETURN_VAL(testdir, NULL);

   l = strlen(testdir);

   p = calloc(1, sizeof(Pommedapi));
   EINA_SAFETY_ON_NULL_RETURN_VAL(p, NULL);

   p->path.testdir = strdup(testdir);
   EINA_SAFETY_ON_NULL_GOTO(p->path.testdir, free_p);

   p->path.conf = utils_strdupf("%s%spommedapi.conf",
                                testdir, testdir[l] == '/' ? "" : "/");
   EINA_SAFETY_ON_NULL_GOTO(p->path.conf, free_p);

   DBG("p->path.testdir[%s] p->path.conf[%s]", p->path.testdir, p->path.conf);

   p->conf = serialize_file_to_struct(p->path.conf, SERIALIZATION_POMMEDAPI_CONF);
   EINA_SAFETY_ON_NULL_GOTO(p->conf, free_p);

   return p;

free_p:
   pommedapi_free(p);
   return NULL;
}
