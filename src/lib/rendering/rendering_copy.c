#include "rendering_private.h"

Eina_Bool
_rendering_copy_file(
   const char *src,
   const char *dst,
   const char *padding,
   const char *filename)
{
   char *s;

   s = rendering_utils_strdupf("%s/%s", dst, padding);
   EINA_SAFETY_ON_NULL_RETURN_VAL(s, EINA_FALSE);

   DBG("Creating %s", s);
   mkdir(s, 0755);
   free(s);

   s = rendering_utils_strdupf("%s/%s%s", dst, padding, filename);
   EINA_SAFETY_ON_NULL_RETURN_VAL(s, EINA_FALSE);

   DBG("Copy [%s] to [%s]", src, s);
   return gfile_copy(src, s);
}

Eina_Bool
_rendering_copy_create_dir(
   const char *dir,
   const char *padding)
{
   char *s;

   s = rendering_utils_strdupf("%s/%s", dir, padding);
   EINA_SAFETY_ON_NULL_RETURN_VAL(s, EINA_FALSE);

   mkdir(s, 0755);
   free(s);
   return EINA_TRUE;
}


Eina_Bool
rendering_copy(
   Rendering *r)
{
   char *dir;
   Eina_Bool ret;

   ret = _rendering_copy_create_dir(r->dir, "/data/");
   EINA_SAFETY_ON_TRUE_RETURN_VAL(!ret, EINA_FALSE);

   ret = _rendering_copy_create_dir(r->dir, "/data/js/");
   EINA_SAFETY_ON_TRUE_RETURN_VAL(!ret, EINA_FALSE);

   ret = _rendering_copy_create_dir(r->dir, "/data/css/");
   EINA_SAFETY_ON_TRUE_RETURN_VAL(!ret, EINA_FALSE);

   ret = _rendering_copy_file(DATA_DIR"/pommedapi/js/bootstrap.min.js"  , r->dir, "data/js/" , "bootstrap.min.js");
   EINA_SAFETY_ON_TRUE_RETURN_VAL(!ret, EINA_FALSE);

   ret = _rendering_copy_file(DATA_DIR"/pommedapi/js/jquery.min.js"     , r->dir, "data/js/" , "jquery.min.js");
   EINA_SAFETY_ON_TRUE_RETURN_VAL(!ret, EINA_FALSE);

   ret = _rendering_copy_file(DATA_DIR"/pommedapi/js/highlight.min.js"     , r->dir, "data/js/" , "highlight.min.js");
   EINA_SAFETY_ON_TRUE_RETURN_VAL(!ret, EINA_FALSE);

   ret = _rendering_copy_file(DATA_DIR"/pommedapi/css/bootstrap.min.css", r->dir, "data/css/", "bootstrap.min.css");
   EINA_SAFETY_ON_TRUE_RETURN_VAL(!ret, EINA_FALSE);

   ret = _rendering_copy_file(DATA_DIR"/pommedapi/css/monokai-sublime.css", r->dir, "data/css/", "monokai-sublime.css");
   EINA_SAFETY_ON_TRUE_RETURN_VAL(!ret, EINA_FALSE);

   return EINA_TRUE;
}

