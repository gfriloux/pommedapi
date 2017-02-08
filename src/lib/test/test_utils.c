#define _GNU_SOURCE
#include <stdio.h>

#include "test_private.h"

float
test_utils_difftime(
   struct timeval start,
   struct timeval end)
{
   return (end.tv_sec  - start.tv_sec ) * 1000.0f +
          (end.tv_usec - start.tv_usec) / 1000.0f;
}


