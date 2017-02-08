#include <Serialize.h>
#include <Azy.h>
#include <Json_Common_Azy.h>
#include <Generic.h>

extern int _serialize_log_dom_global;

#define ERR(...) EINA_LOG_DOM_ERR(_serialize_log_dom_global, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(_serialize_log_dom_global, __VA_ARGS__)
#define NFO(...) EINA_LOG_DOM_INFO(_serialize_log_dom_global, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(_serialize_log_dom_global, __VA_ARGS__)
#define CRI(...) EINA_LOG_DOM_CRIT(_serialize_log_dom_global, __VA_ARGS__)
