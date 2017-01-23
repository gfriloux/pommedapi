#include <Eina.h>
#include <Ecore.h>
#include <Template.h>

extern int _template_log_dom_global;

typedef enum _Template_Type
{
   TEMPLATE_TYPE_FUNCTION,
   TEMPLATE_TYPE_TEMPLATE
} Template_Type;

typedef struct _Template_Function
{
   char *(*function_get)(void *data);
   void *data;
} Template_Function;

typedef struct _Template_Template
{
   Template *template;
} Template_Template;

typedef struct _Template_Word
{
   Template_Type type;
   const char *word;

   union
   {
      Template_Function f;
      Template_Template t;
   } u;
} Template_Word;

struct _Template
{
   const char *file;
   Eina_List *words;
};

#define ERR(...) EINA_LOG_DOM_ERR(_template_log_dom_global, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(_template_log_dom_global, __VA_ARGS__)
#define NFO(...) EINA_LOG_DOM_INFO(_template_log_dom_global, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(_template_log_dom_global, __VA_ARGS__)
#define CRI(...) EINA_LOG_DOM_CRIT(_template_log_dom_global, __VA_ARGS__)
