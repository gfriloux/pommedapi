#include <stddef.h>
#include <Eina.h>

Eina_Bool gfile_data_write(const char *file, const char *data, size_t size);
char * gfile_data_read(const char *file, size_t *size);
Eina_Bool gfile_copy(const char *src, const char *dest);
