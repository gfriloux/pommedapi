#ifndef TEST_H
#define TEST_H
#include <Eina.h>
#include <Serialize.h>

typedef struct _Test
{
   Json_Query *conf;

   struct
   {
      const char *path,
                 *file,
                 *url;
      unsigned int name_start;
      struct timeval time;
      struct
      {
         char *data;
         size_t len;
      } data;

      Eina_List *headers;
   } query;

   struct
   {
      struct timeval time;
      int code;
      float latency;

      struct
      {
         Eina_Strbuf *buf;
      } data;

      Eina_List *headers;
   } result;
} Test;

typedef void (*Test_Done_Cb)(void *data, Test *t);
typedef void (*Test_Error_Cb)(void *data, Test *t, const char *error);

int test_init(void);
int test_shutdown(void);

Test * test_new(const char *host, unsigned int port, const Eina_File_Direct_Info *info);
void test_free(Test *t);

Eina_Bool test_run(Test *t, Test_Done_Cb done, Test_Error_Cb error, void *data);
#endif
