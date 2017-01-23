#define _GNU_SOURCE
#include <stdio.h>

#include "test_private.h"

char *
test_utils_strdupf(
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

float
test_utils_difftime(
   struct timeval start,
   struct timeval end)
{
   return (end.tv_sec  - start.tv_sec ) * 1000.0f +
          (end.tv_usec - start.tv_usec) / 1000.0f;
}


