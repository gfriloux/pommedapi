EXTRA_DIST +=                                                                  \
   data/html/data/css/bootstrap.min.css                                        \
   data/html/data/css/monokai-sublime.css                                      \
   data/html/data/js/bootstrap.min.js                                          \
   data/html/data/js/jquery.min.js                                             \
   data/html/data/js/highlight.min.js                                          \
   data/html/data/fonts/glyphicons-halflings-regular.eot                       \
   data/html/data/fonts/glyphicons-halflings-regular.svg                       \
   data/html/data/fonts/glyphicons-halflings-regular.ttf                       \
   data/html/data/fonts/glyphicons-halflings-regular.woff                      \
   data/html/data/fonts/glyphicons-halflings-regular.woff2                     \
   data/templates/output.tpl                                                   \
   data/templates/test.tpl                                                     \
   data/templates/stats.tpl                                                    \
   data/functions.sh

pommedapidir = $(datarootdir)/pommedapi/
pommedapi_SCRIPTS =                                                            \
   data/functions.sh

tpldir = $(datarootdir)/pommedapi/templates/
tpl_DATA =                                                                     \
   data/templates/output.tpl                                                   \
   data/templates/test.tpl                                                     \
   data/templates/stats.tpl

jsdir = $(datarootdir)/pommedapi/js/
js_DATA =                                                                      \
   data/html/data/js/bootstrap.min.js                                          \
   data/html/data/js/jquery.min.js                                             \
   data/html/data/js/highlight.min.js

cssdir = $(datarootdir)/pommedapi/css/
css_DATA =                                                                     \
   data/html/data/css/bootstrap.min.css                                        \
   data/html/data/css/monokai-sublime.css

fontsdir = $(datarootdir)/pommedapi/fonts/
fonts_DATA =                                                                   \
   data/html/data/fonts/glyphicons-halflings-regular.eot                       \
   data/html/data/fonts/glyphicons-halflings-regular.svg                       \
   data/html/data/fonts/glyphicons-halflings-regular.ttf                       \
   data/html/data/fonts/glyphicons-halflings-regular.woff                      \
   data/html/data/fonts/glyphicons-halflings-regular.woff2
