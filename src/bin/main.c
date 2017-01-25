#include "pommedapi.h"

#define _CRI_GOTO(_a, _b)                                                      \
   do {                                                                        \
      CRI(_a);                                                                 \
      exit_code = 1;                                                           \
      goto _b;                                                                 \
   } while(0)

int _pommedapi_log_dom_global = -1;

static const Ecore_Getopt optdesc = {
   "pommedapi",
   "%prog -d testsdir",
   PACKAGE_VERSION,
   "(C) Guillaume Friloux",
   "BSD.",
   "Web API tester",
   0,
   {
      ECORE_GETOPT_STORE_STR('t', "testdir", "Directory of tests to run."),
      ECORE_GETOPT_STORE_STR('o', "output", "Directory to use for HTML output"),
      ECORE_GETOPT_LICENSE('L', "license"),
      ECORE_GETOPT_COPYRIGHT('C', "copyright"),
      ECORE_GETOPT_VERSION('V', "version"),
      ECORE_GETOPT_HELP('h', "help"),
      ECORE_GETOPT_SENTINEL
   }
};

int
main(
   int argc,
   const char **argv)
{
   char *opt_test = NULL,
        *opt_html = NULL;
   Eina_Bool opt_quit;
   int exit_code = 0;
   Pommedapi *p;

   eina_init();

   Ecore_Getopt_Value values[] = {
     ECORE_GETOPT_VALUE_STR (opt_test),
     ECORE_GETOPT_VALUE_STR (opt_html),
     ECORE_GETOPT_VALUE_BOOL(opt_quit),
     ECORE_GETOPT_VALUE_BOOL(opt_quit),
     ECORE_GETOPT_VALUE_BOOL(opt_quit),
     ECORE_GETOPT_VALUE_BOOL(opt_quit),
     ECORE_GETOPT_VALUE_NONE
   };

   _pommedapi_log_dom_global = eina_log_domain_register("pommedapi", EINA_COLOR_GREEN);
   if (_pommedapi_log_dom_global < 0)
     _CRI_GOTO("Pommedapi can not create a general log domain.", shutdown_eina);

   ecore_app_args_set(argc, argv);

   if (ecore_getopt_parse(&optdesc, values, argc, (char **)argv) < 0)
     _CRI_GOTO("Failed to parse args", unregister_log_domain);

   serialize_init();
   http_init();
   rendering_init();
   test_init();

   p = pommedapi_new(opt_test, opt_html);
   EINA_SAFETY_ON_NULL_GOTO(p, shutdown_test);

   eio_file_stat_ls(p->path.test, list_filter, list_main,
                    list_done, list_error, p);

   ecore_main_loop_begin();

   pommedapi_free(p);

shutdown_test:
   test_shutdown();
   rendering_shutdown();
   http_shutdown();
   serialize_shutdown();
unregister_log_domain:
   eina_log_domain_unregister(_pommedapi_log_dom_global);
   _pommedapi_log_dom_global = -1;
shutdown_eina:
   eina_shutdown();
   return exit_code;
}
