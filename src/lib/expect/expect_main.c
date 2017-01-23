#include "expect_private.h"

static int _expect_init_count     = 0;
int        _expect_log_dom_global = -1;

Expect_Level
expect_test_latency(Test *t)
{
   if (!t->conf->expect->time) return EXPECT_SUCCESS;

   if (t->conf->expect->time > t->result.latency)
     return EXPECT_SUCCESS;
   return EXPECT_WARNING;
}

Expect_Level
expect_test_http_code(Test *t)
{
   if (!t->conf->expect->http_code) return EXPECT_SUCCESS;

   if (t->conf->expect->http_code == t->result.code)
     return EXPECT_SUCCESS;

   if ((t->result.code > 200) && (t->result.code < 400))
     return EXPECT_WARNING;

   return EXPECT_DANGER;
}

Expect_Level
expect_test(Test *t)
{
   Expect_Level level = EXPECT_SUCCESS,
                test;

   test = expect_test_http_code(t);
   if (level < test) level = test;

   test = expect_test_latency(t);
   if (level < test) level = test;

   return level;
}

int
expect_init(
   void)
{
   if (++_expect_init_count != 1)
     return _expect_init_count;

   if (!eina_init())
     {
        fprintf(stderr, "Expect can not initialize Eina\n");
        return --_expect_init_count;
     }

   _expect_log_dom_global = eina_log_domain_register("expect", EINA_COLOR_GREEN);
   if (_expect_log_dom_global < 0)
     {
        EINA_LOG_ERR("Expect can not creation a general log domain.");
        goto shutdown_eina;
     }

   return _expect_init_count;

shutdown_eina:
   eina_shutdown();
   return --_expect_init_count;
}

int
expect_shutdown(
   void)
{
   if (_expect_init_count <= 0)
     {
        fprintf(stderr, "Expect init count not greater than 0 in shutdown.");
        return 0;
     }

   if (--_expect_init_count != 0)
     return _expect_init_count;

   eina_log_domain_unregister(_expect_log_dom_global);
   _expect_log_dom_global = -1;

   eina_shutdown();
   return _expect_init_count;
}
