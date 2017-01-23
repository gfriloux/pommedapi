#ifndef TEMPLATE_H
# define TEMPLATE_H

typedef struct _Template Template;

const char * template_parse(Template *template);
void template_file_set(Template *template, const char *file);
void template_word_del(Template *template, const char *word);
Eina_Bool template_template_add(Template *template, const char *word, Template *template_child);
Eina_Bool template_function_add(Template *template, const char *word, char *(*function)(void *data), void *data);
Template * template_new(const char *file);
void template_free(Template *template);
int template_init(void);
int template_shutdown(void);

#endif
