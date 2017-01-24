#include "rendering_private.h"

char *
rendering_stats_success_count(
   void *data)
{
   Eina_List *tests = data,
             *l;
   Test *t;
   unsigned int success = 0;

   EINA_LIST_FOREACH(tests, l, t)
     if (expect_test(t) == EXPECT_SUCCESS) success++;
   return rendering_utils_strdupf("%u", success);
}

char *
rendering_stats_warning_count(
   void *data)
{
   Eina_List *tests = data,
             *l;
   Test *t;
   unsigned int warning = 0;

   EINA_LIST_FOREACH(tests, l, t)
     if (expect_test(t) == EXPECT_WARNING) warning++;
   return rendering_utils_strdupf("%u", warning);
}

char *
rendering_stats_danger_count(
   void *data)
{
   Eina_List *tests = data,
             *l;
   Test *t;
   unsigned int danger = 0;

   EINA_LIST_FOREACH(tests, l, t)
     if (expect_test(t) == EXPECT_DANGER) danger++;
   return rendering_utils_strdupf("%u", danger);
}

char *
rendering_stats_disable_count(
   void *data)
{
   Eina_List *tests = data,
             *l;
   Test *t;
   unsigned int disabled = 0;

   EINA_LIST_FOREACH(tests, l, t)
     if (expect_test(t) == EXPECT_DISABLE) disabled++;
   return rendering_utils_strdupf("%u", disabled);
}

const char *
rendering_stats(
   Rendering *r,
   Eina_List *tests,
   const char *file)
{
   Template *tpl;
   const char *s;

   DBG("r[%p]", r);

   tpl = template_new(file);
   EINA_SAFETY_ON_NULL_RETURN_VAL(tpl, NULL);

   template_function_add(tpl, "$$STAT_SUCCESS$$", rendering_stats_success_count, tests);
   template_function_add(tpl, "$$STAT_WARNING$$", rendering_stats_warning_count, tests);
   template_function_add(tpl, "$$STAT_DANGER$$" , rendering_stats_danger_count , tests);
   template_function_add(tpl, "$$STAT_DISABLE$$", rendering_stats_disable_count, tests);

   s = template_parse(tpl);

   template_free(tpl);
   return s;
}
