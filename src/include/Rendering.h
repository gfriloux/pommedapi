#include <Eina.h>
#include <Test.h>

typedef struct _Rendering
{
   Eina_Strbuf *buf;

   const char *dir,
              *filename,
              *description;
} Rendering;

int rendering_init(void);
int rendering_shutdown(void);

Rendering * rendering_new(const char *directory, const char *filename, const char *description);
void rendering_free(Rendering *r);

Eina_Bool rendering_output(Rendering *r, Eina_List *tests);
