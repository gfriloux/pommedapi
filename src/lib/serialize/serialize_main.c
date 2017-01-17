#include "serialize_private.h"

static int _serialize_init_count     = 0;
int        _serialize_log_dom_global = -1;

void
serialize_free(
   void *data,
   Serialization_Type type)
{

#define _SERIALIZATION_FREE(_a, _b)                                            \
   case _a:                                                                    \
     {                                                                         \
        _b##_free(data);                                                       \
        break;                                                                 \
     }

#define _SERIALIZATION_ARRAY_FREE(_a, _b)                                      \
   case _a:                                                                    \
     {                                                                         \
        Array_##_b##_free(data);                                               \
        break;                                                                 \
     }

   switch (type)
     {
      _SERIALIZATION_FREE(SERIALIZATION_QUERY, Json_Query)
      _SERIALIZATION_FREE(SERIALIZATION_POMMEDAPI_CONF, Json_Pommedapi_Conf)
      case SERIALIZATION_NONE:
        break;
      default:
        {
           ERR("Unknown serialiation type");
           return;
        }
     }

#undef _SERIALIZATION_FREE
#undef _SERIALIZATION_ARRAY_FREE
}

void *
serialize_string_to_struct(
   const char *s,
   size_t len,
   Serialization_Type type)
{
   Azy_Content *ac;
   Eina_Bool r;
   Eina_Value *ev;
   void *data = NULL;

   ac = azy_content_new(NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ac, NULL);

   r = azy_content_deserialize_json(ac, s, len);
   EINA_SAFETY_ON_TRUE_GOTO(!r, free_ac);

   ev = azy_content_retval_get(ac);
   EINA_SAFETY_ON_NULL_GOTO(ev, free_ac);

#define _SERIALIZATION(_a, _b)                                                 \
   case _a:                                                                    \
     {                                                                         \
        azy_value_to_##_b(ev, (_b **)&data);                                   \
        break;                                                                 \
     }

#define _SERIALIZATION_ARRAY(_a, _b)                                           \
   case _a:                                                                    \
     {                                                                         \
        azy_value_to_Array_##_b(ev, (Eina_List **)&data);                      \
        break;                                                                 \
     }

   switch (type)
     {
      _SERIALIZATION(SERIALIZATION_QUERY, Json_Query)
      _SERIALIZATION(SERIALIZATION_POMMEDAPI_CONF, Json_Pommedapi_Conf)
      default:
        {
           ERR("Unknown serialisation type");
           break;
        }
     }

#undef _SERIALIZATION
#undef _SERIALIZATION_ARRA

free_ac:
   azy_content_free(ac);
   return data;
}

char *
serialize_struct_to_string(
   void *data,
   Serialization_Type type)
{
   char *s;
   Eina_Value *ev = NULL;

#define _SERIALIZATION(_a, _b)                                                 \
   case _a:                                                                    \
     {                                                                         \
        ev = _b##_to_azy_value(data);                                          \
        break;                                                                 \
     }

   switch (type)
     {
      _SERIALIZATION(SERIALIZATION_QUERY, Json_Query)
      _SERIALIZATION(SERIALIZATION_POMMEDAPI_CONF, Json_Pommedapi_Conf)
      default:
        {
           ERR("Unknown serialisation type");
           return NULL;
        }
     }

#undef _SERIALIZATION

   s = azy_content_serialize_json(ev);
   eina_value_free(ev);
   EINA_SAFETY_ON_NULL_RETURN_VAL(s, NULL);

   return s;
}

void *
serialize_file_to_struct(
   const char *file,
   Serialization_Type type)
{
   char *s;
   size_t l;
   void *data;

   s = gfile_data_read(file, &l);
   EINA_SAFETY_ON_NULL_RETURN_VAL(s, NULL);

   data = serialize_string_to_struct(s, l, type);
   free(s);
   return data;
}

Eina_Bool
serialize_struct_to_file(
   void *data,
   const char *file,
   Serialization_Type type)
{
   char *s;
   Eina_Bool r;

   s = serialize_struct_to_string(data, type);
   EINA_SAFETY_ON_NULL_RETURN_VAL(s, EINA_FALSE);

   r = gfile_data_write(file, s, strlen(s));
   free(s);
   EINA_SAFETY_ON_TRUE_RETURN_VAL(!r, EINA_FALSE);

   return EINA_TRUE;
}

int
serialize_init(
   void)
{
   if (++_serialize_init_count != 1)
     return _serialize_init_count;

   if (!eina_init())
     {
        printf("Serialize can not initialize Eina\n");
        return --_serialize_init_count;
     }

   _serialize_log_dom_global = eina_log_domain_register("Serialize", EINA_COLOR_YELLOW);
   if (_serialize_log_dom_global < 0)
     {
        EINA_LOG_ERR("Serialize can not create a general log domain.");
        goto shutdown_eina;
     }

   if (!azy_init())
     {
        EINA_LOG_ERR("Serialize can not initialize Azy.");
        goto unregister_log_domain;
     }

   return _serialize_init_count;

unregister_log_domain:
   eina_log_domain_unregister(_serialize_log_dom_global);
   _serialize_log_dom_global = -1;
shutdown_eina:
   eina_shutdown();
   return --_serialize_init_count;
}

int
serialize_shutdown(
   void)
{
   if (_serialize_init_count <= 0)
     {
        fprintf(stderr, "Serialize init count not greater than 0 in shutdown.");
        return 0;
     }

   if (--_serialize_init_count != 0)
     return _serialize_init_count;

   eina_log_domain_unregister(_serialize_log_dom_global);
   _serialize_log_dom_global = -1;
   eina_shutdown();
   return _serialize_init_count;
}
