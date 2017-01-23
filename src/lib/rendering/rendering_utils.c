#define _GNU_SOURCE
#include <stdio.h>

#include "rendering_private.h"

char *
rendering_utils_strdupf(
   const char *s,
   ...)
{
   va_list args;
   int     len;
   char   *str;

   va_start(args, s);
   len = vasprintf(&str, s, args);
   va_end(args);

   EINA_SAFETY_ON_TRUE_RETURN_VAL(len == -1, NULL);
   return str;
}

