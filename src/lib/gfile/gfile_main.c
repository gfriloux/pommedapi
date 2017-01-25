#include "gfile_private.h"

Eina_Bool
gfile_fd_write(
   int         fd,
   const char *data,
   size_t      size)
{
   ssize_t nb;
   size_t  offset = 0;

   while (offset != size)
     {
        nb = write(fd, data + offset, size - offset);
        EINA_SAFETY_ON_TRUE_RETURN_VAL(nb == -1, EINA_FALSE);

        offset += nb;
     }
   return EINA_TRUE;
}

Eina_Bool
gfile_data_write(
   const char *file,
   const char *data,
   size_t      size)
{
   int     fd = -1;
   char    tmp[strlen(file) + 5];
   ssize_t nb;
   size_t  offset = 0;

   sprintf(tmp, "%s.tmp", file);

   fd = open(tmp, O_WRONLY | O_CREAT | O_TRUNC, 0600);
   EINA_SAFETY_ON_TRUE_RETURN_VAL(fd == -1, EINA_FALSE);

#ifdef _WIN32
   _setmode(fd, _O_BINARY);
#endif

   while (offset != size)
     {
        nb = write(fd, data + offset, size - offset);
        EINA_SAFETY_ON_TRUE_GOTO(nb == -1, remove_tmp);

        offset += nb;
     }

   close(fd);
   fd = -1;

#ifdef _WIN32
   remove(file);
#endif

   EINA_SAFETY_ON_TRUE_GOTO(rename(tmp, file), remove_tmp);
   return EINA_TRUE;

remove_tmp:
   remove(tmp);
close_fd:
   if (fd != -1) close(fd);
   return EINA_FALSE;
}

char *
gfile_data_read(
   const char *file,
   size_t     *size)
{
   Eina_File *ef;
   char      *p;
   char      *s;

   EINA_SAFETY_ON_NULL_RETURN_VAL(file, NULL);

   ef = eina_file_open(file, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ef,   NULL);

   p = (char*)eina_file_map_all(ef, EINA_FILE_SEQUENTIAL);
   EINA_SAFETY_ON_NULL_GOTO(p, free_ef);

   *size = eina_file_size_get(ef);

#ifdef _WIN32
   s = gfile_strndup(p, *size);
#else
   s = strndup(p, *size);
#endif
   EINA_SAFETY_ON_NULL_GOTO(s, free_ef);

   eina_file_map_free(ef, (void*)p);
   eina_file_close(ef);

   return s;

free_ef:
   eina_file_close(ef);
   return NULL;
}

#ifdef _WIN32
/*
 * Evil is supposed to export strndup, but it seems
 * it doesnt work out well. Dunno why is working on my
 * machine, but not on the jenkins node.
 * So, Lets add this function !
 */
char *
gfile_strndup(
   const char *s,
   size_t      n)
{
   char  *p;
   size_t l = strlen(s);

   if (l > n) l = n;

   p = (char*)malloc(l + 1);
   if (!p) return NULL;

   p[l] = '\0';
   return (char*)memcpy(p, s, l);
}
#endif

Eina_Bool
gfile_copy(
   const char *src,
   const char *dest)
{
   Eina_Bool r;
   char *s;
   size_t size;

   s = gfile_data_read(src, &size);
   EINA_SAFETY_ON_NULL_RETURN_VAL(s, EINA_FALSE);

   r = gfile_data_write(dest, s, size);
   free(s);
   EINA_SAFETY_ON_FALSE_RETURN_VAL(r, EINA_FALSE);

   return EINA_TRUE;
}

