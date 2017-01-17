#include <Eina.h>

typedef struct _Http Http;

typedef struct _Http_Header
{
   const char *key,
              *value;
} Http_Header;

typedef void (*Http_Done_Cb)(void *data, Http *http);
typedef void (*Http_Progress_Cb)(void *data, char *url_data, size_t url_size);
typedef void (*Http_Error_Cb)(void *data, const char *error);

int http_init(void);
int http_shutdown(void);

int http_status_code_get(Http *http);
const Eina_List * http_headers_get(Http *http);
Eina_Bool http_data_post(const char *url, Eina_List *headers, Http_Progress_Cb progress_cb, Http_Done_Cb done_cb, Http_Error_Cb error_cb, void *post_data, long post_len, const char *content_type, void *data);
Eina_Bool http_data_download(const char *url, Eina_List *headers, Http_Progress_Cb progress_cb, Http_Done_Cb done_cb, Http_Error_Cb error_cb, void *data);
Eina_Bool http_file_download(const char *url, Eina_List *headers, const char *file, Http_Done_Cb done_cb, Http_Error_Cb error_cb, void *data);

Http_Header * http_header_new(const char *key, const char *value);
void http_header_free(Http_Header *hh);
