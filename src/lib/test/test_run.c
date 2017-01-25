#include "test_private.h"
#include <unistd.h>

void
test_run_free(
   Test_Run *tr)
{
   if (tr->ev.data) ecore_event_handler_del(tr->ev.data);
   if (tr->ev.data) ecore_event_handler_del(tr->ev.del);

   if (tr->tmpfile.fd > 0) close(tr->tmpfile.fd);
   remove(tr->tmpfile.path);
   free(tr->tmpfile.path);

   free(tr);
}

Test_Run *
test_run_new(
   Test *t,
   Test_Done_Cb done,
   Test_Error_Cb error,
   void *data)
{
   Test_Run *tr;

   tr = calloc(1, sizeof(Test_Run));
   EINA_SAFETY_ON_NULL_RETURN_VAL(tr, NULL);

   tr->tmpfile.path = strdup("/tmp/pommedapi.XXXXXX");
   EINA_SAFETY_ON_NULL_GOTO(tr->tmpfile.path, free_tr);

   tr->tmpfile.fd = mkstemp(tr->tmpfile.path);
   EINA_SAFETY_ON_TRUE_GOTO(tr->tmpfile.fd == -1, free_tr);

   tr->t        = t;
   tr->cb.done  = done;
   tr->cb.error = error;
   tr->cb.data  = data;

   return tr;

free_tr:
   test_run_free(tr);
   return NULL;
}

Eina_Bool
_test_run_validate_data(
   void *data,
   int type,
   void *event)
{
   Test_Run *tr            = data;
   Ecore_Exe_Event_Data *d = event;

   if (data != ecore_exe_data_get(d->exe))
     return EINA_TRUE;

   eina_strbuf_append_length(tr->t->validate.output, d->data, d->size);

   return EINA_TRUE;
}

Eina_Bool
_test_run_validate_del(
   void *data,
   int type,
   void *event)
{
   Test_Run *tr           = data;
   Ecore_Exe_Event_Del *d = event;

   if (data != ecore_exe_data_get(d->exe))
     return EINA_TRUE;

   tr->t->validate.exit_code = d->exit_code;

   tr->cb.done(tr->cb.data, tr->t);
   test_run_free(tr);
   return EINA_TRUE;
}

void
test_run_progress(
   void  *data,
   char  *url_data,
   size_t url_size)
{
   Test_Run *tr = data;

   DBG("url_data[%p] url_size[%zu]", url_data, url_size);
   eina_strbuf_append_length(tr->t->result.data.buf, url_data, url_size);
}

void
test_run_done(
   void *data,
   Http *http)
{
   Test_Run *tr = data;
   Ecore_Exe *exe;
   char *cmd;

   gettimeofday(&tr->t->result.time, 0);
   tr->t->result.latency = test_utils_difftime(tr->t->query.time, tr->t->result.time);
   tr->t->result.code = http_status_code_get(http);

   DBG("t[%p] : Got HTTP code %d and %zu bytes of data in %.02fms",
       tr->t, tr->t->result.code, eina_strbuf_length_get(tr->t->result.data.buf), tr->t->result.latency);

   if (!tr->t->validate.file)
     {
done_cb:
        tr->cb.done(tr->cb.data, tr->t);
        test_run_free(tr);
        return;
     }

   tr->t->validate.exit_code = 1;

   tr->ev.data  = ecore_event_handler_add(ECORE_EXE_EVENT_DATA , _test_run_validate_data , tr);
   tr->ev.del   = ecore_event_handler_add(ECORE_EXE_EVENT_DEL  , _test_run_validate_del  , tr);

   gfile_fd_write(tr->tmpfile.fd, eina_strbuf_string_get(tr->t->result.data.buf), eina_strbuf_length_get(tr->t->result.data.buf));

   cmd = test_utils_strdupf("%s '%s'", tr->t->validate.file, tr->tmpfile.path);
   EINA_SAFETY_ON_NULL_GOTO(cmd, done_cb);

   // Running validation script.
   exe = ecore_exe_pipe_run(cmd, ECORE_EXE_PIPE_READ, tr);
   free(cmd);
   EINA_SAFETY_ON_NULL_GOTO(exe, done_cb);
   ecore_exe_data_set(exe, tr);
}

void
test_run_error(
   void *data,
   const char *error)
{
   Test_Run *tr = data;

   tr->cb.error(tr->cb.data, tr->t, error);
   test_run_free(tr);
}
