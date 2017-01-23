#include "rendering_private.h"

char *
rendering_test_url(
   void *data)
{
   Test *t = data;
   DBG("t[%p]", t);
   return strdup(t->query.url);
}

char *
rendering_test_code(
   void *data)
{
   Test *t = data;
   DBG("t[%p]", t);
   return rendering_utils_strdupf("%d", t->result.code);
}

char *
rendering_test_latency(
   void *data)
{
   Test *t = data;
   DBG("t[%p]", t);
   return rendering_utils_strdupf("%.02fms", t->result.latency);
}

char *
rendering_test_size(
   void *data)
{
   Test *t = data;
   DBG("t[%p]", t);
   return rendering_utils_strdupf("%zu bytes", eina_strbuf_length_get(t->result.data.buf));
}

char *
rendering_test_query(
   void *data)
{
   Test *t = data;
   DBG("t[%p]", t);
   return strndup(t->query.data.data, t->query.data.len);
}

char *
rendering_test_result(
   void *data)
{
   Test *t = data;
   DBG("t[%p]", t);
   return strdup(eina_strbuf_string_get(t->result.data.buf));
}

const char *
rendering_test(
   Rendering *r,
   Test *t,
   const char *file)
{
   Template *tpl;
   const char *s;

   DBG("r[%p] t[%p]", r, t);

   tpl = template_new(file);
   EINA_SAFETY_ON_NULL_RETURN_VAL(tpl, NULL);

   template_function_add(tpl, "$$URL$$", rendering_test_url, t);
   template_function_add(tpl, "$$CODE$$", rendering_test_code, t);
   template_function_add(tpl, "$$LATENCY$$", rendering_test_latency, t);
   template_function_add(tpl, "$$SIZE$$", rendering_test_size, t);
   template_function_add(tpl, "$$QUERY$$", rendering_test_query, t);
   template_function_add(tpl, "$$RESULT$$", rendering_test_result, t);

   s = template_parse(tpl);

   template_free(tpl);
   return s;
}
