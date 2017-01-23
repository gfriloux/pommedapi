#ifndef SERIALIZE_H
#define SERIALIZE_H
# include <stddef.h>
# include <Eina.h>
# include <Json_Common_Azy.h>

typedef enum _Serialization_Type
{
   SERIALIZATION_QUERY,
   SERIALIZATION_POMMEDAPI_CONF,
   SERIALIZATION_NONE
} Serialization_Type;

int serialize_init(void);
int serialize_shutdown(void);

void * serialize_string_to_struct(const char *s, size_t len, Serialization_Type type);
char * serialize_struct_to_string(void *data, Serialization_Type type);

Eina_Bool serialize_struct_to_file(void *data, const char *file, Serialization_Type type);
void * serialize_file_to_struct(const char *file, Serialization_Type type);

void serialize_free(void *data, Serialization_Type type);
#endif
