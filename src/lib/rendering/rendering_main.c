#include "rendering_private.h"

#include <unistd.h>

static int _rendering_init_count     = 0;
int        _rendering_log_dom_global = -1;

char *
_rendering_output_strdup(
   void *data)
{
   return strdup(data);
}

Eina_Bool
rendering_output(
   Rendering *r,
   Eina_List *tests)
{
   Test *t;
   Eina_List *l;
   Template *tpl;
   Eina_Strbuf *buf;
   const char *s,
              *c,
              *p;
   Eina_Bool ret;

   tpl = template_new(DATA_DIR"/pommedapi/templates/output.tpl");
   EINA_SAFETY_ON_NULL_RETURN_VAL(tpl, EINA_FALSE);

   EINA_LIST_FOREACH(tests, l, t)
     {
        s = rendering_test(r, t, DATA_DIR"/pommedapi/templates/test.tpl");
        EINA_SAFETY_ON_NULL_GOTO(s, end_loop);

        eina_strbuf_append(r->buf, s);
        free((char *)s);
end_loop:
        continue;
     }

   s = eina_strbuf_string_steal(r->buf);
   EINA_SAFETY_ON_NULL_GOTO(s, free_tpl);

   c = rendering_stats(r, tests, DATA_DIR"/pommedapi/templates/stats.tpl");
   EINA_SAFETY_ON_NULL_GOTO(c, free_s);

   template_function_add(tpl, "$$TESTS$$", _rendering_output_strdup, (void *)s);
   template_function_add(tpl, "$$STATS$$", _rendering_output_strdup, (void *)c);

   p = template_parse(tpl);
   free((char *)s); s = NULL;
   free((char *)c); c = NULL;
   EINA_SAFETY_ON_NULL_GOTO(p, free_tpl);

   DBG("%s", p);

   s = rendering_utils_strdupf("%s/index.html", r->dir);
   EINA_SAFETY_ON_NULL_GOTO(s, free_p);

   ret = gfile_data_write(s, (char *)p, strlen(p));
   free((char *)p); p = NULL;
   free((char *)s);
   EINA_SAFETY_ON_TRUE_GOTO(!ret, free_tpl);

   template_free(tpl);
   return rendering_copy(r);

free_p:
   free((char *)p);
   free((char *)c);
free_s:
   free((char *)s);
free_tpl:
   template_free(tpl);
   return EINA_FALSE;
}

void
rendering_free(
   Rendering *r)
{
   EINA_SAFETY_ON_NULL_RETURN(r);
   eina_strbuf_free(r->buf);
   free((char *)r->dir);
   free(r);
}

Rendering *
rendering_new(
   const char *dir)
{
   Rendering *r;
   int ret;

   DBG("dir[%s]", dir);

   r = calloc(1, sizeof(Rendering));
   EINA_SAFETY_ON_NULL_RETURN_VAL(r, NULL);

   r->buf = eina_strbuf_new();
   EINA_SAFETY_ON_NULL_GOTO(r->buf, free_r);

   r->dir = strdup(dir);
   EINA_SAFETY_ON_NULL_GOTO(r->dir, free_r);

   ret = access(dir, W_OK);
   if (!ret) return r;

   ret = access(dir, R_OK);
   EINA_SAFETY_ON_TRUE_GOTO(!ret, free_r);

   ret = mkdir(dir, 0755);
   EINA_SAFETY_ON_TRUE_GOTO(ret, free_r);

   return r;

free_r:
   rendering_free(r);
   return NULL;
}

int
rendering_init(
   void)
{
   if (++_rendering_init_count != 1)
     return _rendering_init_count;

   if (!eina_init())
     {
        fprintf(stderr, "Rendering can not initialize Eina\n");
        return --_rendering_init_count;
     }

   _rendering_log_dom_global = eina_log_domain_register("rendering", EINA_COLOR_RED);
   if (_rendering_log_dom_global < 0)
     {
        EINA_LOG_ERR("Rendering can not create a general log domain.");
        goto shutdown_eina;
     }

   if (!template_init())
     {
        EINA_LOG_ERR("Rendering can not initialize Template");
        goto unregister_log_domain;
     }

   if (!expect_init())
     {
        EINA_LOG_ERR("Expect can not initialize Expect");
        goto shutdown_expect;
     }

   return _rendering_init_count;

shutdown_expect:
   expect_shutdown();
unregister_log_domain:
   eina_log_domain_unregister(_rendering_log_dom_global);
   _rendering_log_dom_global = -1;
shutdown_eina:
   eina_shutdown();
   return --_rendering_init_count;
}

int
rendering_shutdown(
   void)
{
   if (_rendering_init_count <= 0)
     {
        fprintf(stderr, "Rendering init count not greater than 0 in shutdown.");
        return 0;
     }

   if (--_rendering_init_count != 0)
     return _rendering_init_count;

   expect_shutdown();
   template_shutdown();

   eina_log_domain_unregister(_rendering_log_dom_global);
   _rendering_log_dom_global = -1;

   eina_shutdown();
   return _rendering_init_count;
}
