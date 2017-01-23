#include <Test.h>
#include <Http.h>

#include <Gfile.h>
#include <Generic.h>

typedef struct _Test_Run
{
   Test *t;

   struct
   {
      Test_Done_Cb done;
      Test_Error_Cb error;
      void *data;
   } cb;
} Test_Run;

extern int _test_log_dom_global;

#define ERR(...) EINA_LOG_DOM_ERR(_test_log_dom_global, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(_test_log_dom_global, __VA_ARGS__)
#define NFO(...) EINA_LOG_DOM_INFO(_test_log_dom_global, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(_test_log_dom_global, __VA_ARGS__)
#define CRI(...) EINA_LOG_DOM_CRIT(_test_log_dom_global, __VA_ARGS__)

Test_Run * test_run_new(Test *t, Test_Done_Cb done, Test_Error_Cb error, void *data);
void test_run_free(Test_Run *tr);

void test_run_progress(void *data, char *url_data, size_t url_size);
void test_run_done(void *data, Http *http);
void test_run_error(void *data, const char *error);

char * test_utils_strdupf(const char *s, ...);
float test_utils_difftime(struct timeval start, struct timeval end);
