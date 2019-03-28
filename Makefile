all:
	cargo build --release

install:
	install -D -s target/release/pommedapi                                /usr/local/bin/pommedapi
	install -D    data/html/data/css/bootstrap.min.css                    /usr/local/share/pommedapi/css/bootstrap.min.css
	install -D    data/html/data/css/monokai-sublime.css                  /usr/local/share/pommedapi/css/monokai-sublime.css
	install -D    data/html/data/js/bootstrap.min.js                      /usr/local/share/pommedapi/js/bootstrap.min.js
	install -D    data/html/data/js/highlight.min.js                      /usr/local/share/pommedapi/js/highlight.min.js
	install -D    data/html/data/js/jquery.min.js                         /usr/local/share/pommedapi/js/jquery.min.js
	install -D    data/html/data/fonts/glyphicons-halflings-regular.eot   /usr/local/share/pommedapi/fonts/glyphicons-halflings-regular.eot
	install -D    data/html/data/fonts/glyphicons-halflings-regular.svg   /usr/local/share/pommedapi/fonts/glyphicons-halflings-regular.svg
	install -D    data/html/data/fonts/glyphicons-halflings-regular.ttf   /usr/local/share/pommedapi/fonts/glyphicons-halflings-regular.ttf
	install -D    data/html/data/fonts/glyphicons-halflings-regular.woff  /usr/local/share/pommedapi/fonts/glyphicons-halflings-regular.woff
	install -D    data/html/data/fonts/glyphicons-halflings-regular.woff2 /usr/local/share/pommedapi/fonts/glyphicons-halflings-regular.woff2
	install -D    data/templates/template.hbs                             /usr/local/share/pommedapi/templates/template.hbs
	install -D    data/functions.sh    /usr/local/share/pommedapi/