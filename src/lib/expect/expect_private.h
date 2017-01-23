#include <Expect.h>
#include <Eina.h>

extern int _expect_log_dom_global;

#define ERR(...) EINA_LOG_DOM_ERR (_expect_log_dom_global, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG (_expect_log_dom_global, __VA_ARGS__)
#define NFO(...) EINA_LOG_DOM_INFO(_expect_log_dom_global, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(_expect_log_dom_global, __VA_ARGS__)
#define CRI(...) EINA_LOG_DOM_CRIT(_expect_log_dom_global, __VA_ARGS__)

