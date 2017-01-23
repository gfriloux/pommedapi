#include "template_private.h"

static int _template_init_count     = 0;
int        _template_log_dom_global = -1;

const char *
template_parse(
   Template *template)
{
   Eina_File     *ef;
   void          *data;
   char          *p;
   char          *p2;
   size_t         len;
   Eina_Strbuf   *buf;
   Eina_List     *l;
   Template_Word *word;

   EINA_SAFETY_ON_NULL_RETURN_VAL(template, NULL);

   DBG("Opening [%s]", template->file);
   ef = eina_file_open(template->file, EINA_FALSE);
   if (!ef)
     {
        ERR( "Failed to open %s : %s", template->file, strerror(errno) );
        return NULL;
     }
   len  = eina_file_size_get(ef);
   data = eina_file_map_all(ef, EINA_FILE_POPULATE);
   EINA_SAFETY_ON_NULL_GOTO(data, close_file);

   buf  = eina_strbuf_new();
   EINA_SAFETY_ON_NULL_GOTO(buf, free_data);

   p = data;

   do
     {
        p2 = strstr(p, "$$");
        if (!p2)
          {
             eina_strbuf_append_length( buf, p, len - (p - (char*)data) );
             break;
          }
        eina_strbuf_append_length(buf, p, p2 - p);
        EINA_LIST_FOREACH (template->words, l, word)
          {
             const char *s;
             size_t      lenvar = strlen(word->word);
             if ( strncmp(p2, word->word, lenvar) )
                continue;

             if (word->type == TEMPLATE_TYPE_FUNCTION)
                s = word->u.f.function_get(word->u.f.data);
             else
                s = template_parse(word->u.t.template);
             eina_strbuf_append(buf, s);
             free( (char*)s );
             p = p2 + lenvar;
          }
        if (p < p2)
          {
             eina_strbuf_append(buf, "$");
             p = p2 + 1;
          }
     } while (1);
   p = eina_strbuf_string_steal(buf);
   eina_strbuf_free(buf);

free_data:
   eina_file_map_free(ef, data);

close_file:
   eina_file_close(ef);
   return p;
}

Eina_Bool
template_template_add(
   Template   *template,
   const char *word,
   Template   *template_child)
{
   Template_Word *template_word;

   EINA_SAFETY_ON_NULL_RETURN_VAL(template,       EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(word,           EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(template_child, EINA_FALSE);

   template_word = calloc( 1, sizeof(Template_Word) );
   if (!template_word)
      return EINA_FALSE;

   template_word->type         = TEMPLATE_TYPE_TEMPLATE;
   template_word->word         = strdup(word);
   template_word->u.t.template = template_child;
   template->words             = eina_list_append(template->words, template_word);
   return EINA_TRUE;
}

void
template_file_set(
   Template   *template,
   const char *file)
{
   EINA_SAFETY_ON_NULL_RETURN(template);
   EINA_SAFETY_ON_NULL_RETURN(file);

   if (template->file) free( (char*)template->file );
   template->file = strdup(file);
}

void
template_word_del(
   Template   *template,
   const char *word)
{
   Eina_List     *l;
   Eina_List     *ln;
   Template_Word *template_word;

   EINA_SAFETY_ON_NULL_RETURN(template);
   EINA_SAFETY_ON_NULL_RETURN(word);

   EINA_LIST_FOREACH_SAFE (template->words, l, ln, template_word)
     {
        if ( strcasecmp(template_word->word, word) ) continue;

        template->words = eina_list_remove(template->words, template_word);
        free( (char*)template_word->word );
        free( template_word);
     }
}

Eina_Bool
template_function_add(
   Template   *template,
   const char *word,
   char *(*function)(void *data),
   void       *data)
{
   Template_Word *template_word;

   EINA_SAFETY_ON_NULL_RETURN_VAL(template, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(word,     EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(function, EINA_FALSE);

   template_word = calloc( 1, sizeof(Template_Word) );
   if (!template_word)
      return EINA_FALSE;

   template_word->type             = TEMPLATE_TYPE_FUNCTION;
   template_word->word             = strdup(word);
   template_word->u.f.function_get = function;
   template_word->u.f.data         = data;
   template->words                 = eina_list_append(template->words, template_word);
   return EINA_TRUE;
}

Template *
template_new(
   const char *file)
{
   Template *template;

   template       = calloc( 1, sizeof(Template) );
   template->file = file ? strdup(file) : NULL;
   return template;
}

void
template_free(
   Template *template)
{
   Template_Word *word;

   EINA_SAFETY_ON_NULL_RETURN(template);

   EINA_LIST_FREE (template->words, word)
     {
        free( (char*)word->word );
        free( word);
     }
   if (template->file) free( (char*)template->file );
   free(template);
}

int
template_init(
   void)
{
   if (++_template_init_count != 1)
      return _template_init_count;

   if ( !eina_init() )
     {
        fprintf(stderr, "Template can not initialize Eina\n");
        return --_template_init_count;
     }

   _template_log_dom_global = eina_log_domain_register("template", EINA_COLOR_WHITE);
   if (_template_log_dom_global < 0)
     {
        EINA_LOG_ERR("Template can not create a general log domain");
        goto shutdown_eina;
     }

   if ( !ecore_init() )
     {
        ERR("Can not initialize Ecore");
        goto unregister_log_domain;
     }

   return _template_init_count;

unregister_log_domain:
   eina_log_domain_unregister(_template_log_dom_global);
   _template_log_dom_global = -1;
shutdown_eina:
   eina_shutdown();
   return --_template_init_count;
}

int
template_shutdown(
   void)
{
   if (_template_init_count <= 0)
     {
        fprintf(stderr, "Template init count not greater than 0 in shutdown.");
        return 0;
     }

   if (--_template_init_count != 0)
      return _template_init_count;

   ecore_shutdown();
   eina_log_domain_unregister(_template_log_dom_global);
   _template_log_dom_global = -1;
   eina_shutdown();
   return _template_init_count;
}
