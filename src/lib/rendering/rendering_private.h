#include <Rendering.h>
#include <Template.h>
#include <Gfile.h>
#include <Expect.h>

extern int _rendering_log_dom_global;

#define ERR(...) EINA_LOG_DOM_ERR (_rendering_log_dom_global, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG (_rendering_log_dom_global, __VA_ARGS__)
#define NFO(...) EINA_LOG_DOM_INFO(_rendering_log_dom_global, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(_rendering_log_dom_global, __VA_ARGS__)
#define CRI(...) EINA_LOG_DOM_CRIT(_rendering_log_dom_global, __VA_ARGS__)

char * rendering_test_url(void *data);
char * rendering_test_code(void *data);
char * rendering_test_latency(void *data);
char * rendering_test_size(void *data);
char * rendering_test_query(void *data);
char * rendering_test_result(void *data);

const char * rendering_test(Rendering *r, Test *t, const char *file);
Eina_Bool rendering_copy(Rendering *r);

char * rendering_utils_strdupf(const char *s, ...);
