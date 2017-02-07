<!doctype html>
<html xmlns="http://www.w3.org/1999/xhtml">
  <head>
    <meta charset="utf-8">
    <title>Pommedapi</title>
    <link href="data/css/bootstrap.min.css" rel="stylesheet">
    <link href="data/css/monokai-sublime.css" rel="stylesheet">
  </head>
  <body>
    <div class="container">
    <nav class="navbar navbar-default" role="navigation">
      <div class="container-fluid">
        <div class="navbar-header">
          <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1">
            <span class="sr-only">Toggle navigation</span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </button>
          <a class="navbar-brand" href="">Pommedapi's Results</a>
        </div>
      </div>
    </nav>

    <div class="content">
      <div class="panel panel-default">
        <div class="panel-heading">Description</div>
          <div class="panel-body">
            $$DESCRIPTION$$
          </div>
        </div>
      </div>

      $$STATS$$

      $$TESTS$$
    </div>
  </body>
  <script src="data/js/jquery.min.js"></script>
  <script src="data/js/bootstrap.min.js"></script>
  <script src="data/js/highlight.min.js"></script>
  <script type="text/javascript">
    hljs.initHighlightingOnLoad();
  </script>
</html>
