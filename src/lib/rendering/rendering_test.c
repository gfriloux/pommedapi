#include "rendering_private.h"

char *
rendering_test_disable(
   void *data)
{
   Test *t = data;

   if (t->conf->disabled) return strdup("default");
   return strdup("primary");
}

char *
rendering_test_description(
   void *data)
{
   Test *t = data;
   Eina_Strbuf *buf;
   char *s;

   DBG("t[%p]", t);

   buf = eina_strbuf_new();
   EINA_SAFETY_ON_NULL_RETURN_VAL(buf, NULL);

   eina_strbuf_append_printf(buf, "%s\n<br />\n", t->conf->description);
   eina_strbuf_append_printf(buf,
                             "<h3>Expected results</h3>\n"
                             "<ul>\n"
                             "  <li>http code : %d</li>\n"
                             "  <li>time : %dms</li>\n"
                             "</ul>\n",
                             t->conf->expect->http_code,
                             t->conf->expect->time);

   s = eina_strbuf_string_steal(buf);
   eina_strbuf_free(buf);

   return s;
}

char *
rendering_test_id(
   void *data)
{
   Test *t = data;
   DBG("t[%p]", t);
   return strdup(t->query.path + t->query.name_start);
}

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
   Eina_Strbuf *buf;
   char *s;
   Expect_Level level;

   DBG("t[%p]", t);

   buf = eina_strbuf_new();
   EINA_SAFETY_ON_NULL_RETURN_VAL(buf, NULL);

   eina_strbuf_append(buf, "<button type=\"button\" class=\"btn ");

   level = expect_test_http_code(t);
   switch (level)
     {
      case EXPECT_DISABLE:
        eina_strbuf_append(buf, "btn-default");
        break;
      case EXPECT_SUCCESS:
        eina_strbuf_append(buf, "btn-success");
        break;
      case EXPECT_WARNING:
        eina_strbuf_append(buf, "btn-warning");
        break;
      case EXPECT_DANGER:
      default:
        eina_strbuf_append(buf, "btn-danger");
     }

   eina_strbuf_append(buf, " btn-xs\">");
   eina_strbuf_append(buf, "<span class=\"glyphicon glyphicon-record\" aria-hidden=\"true\"></span>");
   eina_strbuf_append_printf(buf, "&nbsp;%i", t->result.code);
   eina_strbuf_append(buf, "</button>");

   s = eina_strbuf_string_steal(buf);
   eina_strbuf_free(buf);

   return s;
}

char *
rendering_test_latency(
   void *data)
{
   Test *t = data;
   Eina_Strbuf *buf;
   char *s;
   Expect_Level level;

   DBG("t[%p]", t);

   buf = eina_strbuf_new();
   EINA_SAFETY_ON_NULL_RETURN_VAL(buf, NULL);

   eina_strbuf_append(buf, "<button type=\"button\" class=\"btn ");

   level = expect_test_latency(t);
   switch (level)
     {
      case EXPECT_DISABLE:
        eina_strbuf_append(buf, "btn-default");
        break;
      case EXPECT_SUCCESS:
        eina_strbuf_append(buf, "btn-success");
        break;
      default:
        eina_strbuf_append(buf, "btn-warning");
     }

   eina_strbuf_append(buf, " btn-xs\">");
   eina_strbuf_append(buf, "<span class=\"glyphicon glyphicon-hourglass\" aria-hidden=\"true\"></span>");
   eina_strbuf_append_printf(buf, "&nbsp;%.02fms", t->result.latency);
   eina_strbuf_append(buf, "</button>");

   s = eina_strbuf_string_steal(buf);
   eina_strbuf_free(buf);

   return s;
}

char *
rendering_test_size(
   void *data)
{
   Test *t = data;
   Eina_Strbuf *buf;
   char *s;

   DBG("t[%p]", t);

   buf = eina_strbuf_new();
   EINA_SAFETY_ON_NULL_RETURN_VAL(buf, NULL);

   eina_strbuf_append(buf, "<button type=\"button\" class=\"btn ");
   eina_strbuf_append_printf(buf, "%s btn-xs\">", (t->conf->disabled) ? "btn-default" : "btn-success");
   eina_strbuf_append(buf, "<span class=\"glyphicon glyphicon-download\" aria-hidden=\"true\"></span>");
   eina_strbuf_append_printf(buf, "&nbsp;%zu bytes", eina_strbuf_length_get(t->result.data.buf));
   eina_strbuf_append(buf, "</button>");

   s = eina_strbuf_string_steal(buf);
   eina_strbuf_free(buf);

   return s;
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

   template_function_add(tpl, "$$URL$$"        , rendering_test_url        , t);
   template_function_add(tpl, "$$CODE$$"       , rendering_test_code       , t);
   template_function_add(tpl, "$$LATENCY$$"    , rendering_test_latency    , t);
   template_function_add(tpl, "$$SIZE$$"       , rendering_test_size       , t);
   template_function_add(tpl, "$$QUERY$$"      , rendering_test_query      , t);
   template_function_add(tpl, "$$RESULT$$"     , rendering_test_result     , t);
   template_function_add(tpl, "$$ID$$"         , rendering_test_id         , t);
   template_function_add(tpl, "$$DESCRIPTION$$", rendering_test_description, t);
   template_function_add(tpl, "$$DISABLE$$"    , rendering_test_disable    , t);

   s = template_parse(tpl);

   template_free(tpl);
   return s;
}
