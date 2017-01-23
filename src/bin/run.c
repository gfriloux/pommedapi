#include "pommedapi.h"

void
run_done(
   void *data,
   Test *t)
{
   Pommedapi *p = data;

   DBG("p[%p]", p);

end:
   run_next(p);
}

void
run_error(
   void *data,
   Test *t,
   const char *error)
{
   Pommedapi *p = data;

   ERR("p[%p] t[%p] error[%s]", p, t, error);
   run_next(p);
}

void
run_next(
   Pommedapi *p)
{
   Eina_Bool r;

begin:
   p->current = _EINA_LIST_POP(p->tests.pending);
   if (!p->current)
     {
        DBG("End of tests, Start rendering");
        rendering_output(p->rendering, p->tests.render);
        ecore_main_loop_quit();
        return;
     }

   _EINA_LIST_APPEND(p->tests.render, p->current);

   r = test_run(p->current, run_done, run_error, p);
   EINA_SAFETY_ON_TRUE_GOTO(!r, begin);
}
