#include <Eina.h>

#define _EINA_LIST_APPEND(_a, _b)  _a = eina_list_append(_a, _b)
#define _EINA_LIST_PREPEND(_a, _b) _a = eina_list_prepend(_a, _b)
#define _EINA_LIST_REMOVE(_a, _b)  _a = eina_list_remove(_a, _b)
#define _EINA_LIST_POP(_a)         _eina_list_pop(&_a)

static inline void *
_eina_list_pop(Eina_List **l)
{
   void *data;

   data = eina_list_nth(*l, 0);
   _EINA_LIST_REMOVE(*l, data);
   return data;
}

