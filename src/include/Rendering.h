#include <Eina.h>
#include <Test.h>

typedef struct _Rendering
{
   Eina_Strbuf *buf;

   const char *dir;
} Rendering;

int rendering_init(void);
int rendering_shutdown(void);

Rendering * rendering_new(const char *file);
void rendering_free(Rendering *r);

Eina_Bool rendering_output(Rendering *r, Eina_List *tests);
