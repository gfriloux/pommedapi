EXTRA_DIST +=                                                                  \
   data/html/data/css/bootstrap.min.css                                        \
   data/html/data/css/monokai-sublime.css                                      \
   data/html/data/js/bootstrap.min.js                                          \
   data/html/data/js/jquery.min.js                                             \
   data/html/data/js/highlight.min.js                                          \
   data/templates/output.tpl                                                   \
   data/templates/test.tpl

tpldir = $(datarootdir)/pommedapi/templates/
tpl_DATA =                                                                     \
   data/templates/output.tpl                                                   \
   data/templates/test.tpl

jsdir = $(datarootdir)/pommedapi/js/
js_DATA =                                                                      \
   data/html/data/js/bootstrap.min.js                                          \
   data/html/data/js/jquery.min.js                                             \
   data/html/data/js/highlight.min.js

cssdir = $(datarootdir)/pommedapi/css/
css_DATA =                                                                     \
   data/html/data/css/bootstrap.min.css                                        \
   data/html/data/css/monokai-sublime.css
