<div class="panel panel-$$DISABLE$$">
  <div class="panel-heading">
    <h3 class="panel-title">$$ID$$ - $$URL$$</h3>
  </div>
  <div class="panel-body">
    $$CODE$$

    $$LATENCY$$

    $$SIZE$$

    <br /><br />
    <ul class="nav nav-tabs">
      <li class="active"><a href="#$$ID$$_desc" data-toggle="tab" aria-expanded="true">Description</a></li>
      <li class=""><a href="#$$ID$$_sent" data-toggle="tab" aria-expanded="false">Sent data</a></li>
      <li class=""><a href="#$$ID$$_received" data-toggle="tab" aria-expanded="false">Received data</a></li>
      <li class=""><a href="#$$ID$$_validation" data-toggle="tab" aria-expanded="false">Validation data</a></li>
    </ul>
    <div id="myTabContent" class="tab-content">
      <div class="tab-pane fade active in" id="$$ID$$_desc">
        $$DESCRIPTION$$
      </div>
      <div class="tab-pane fade in" id="$$ID$$_sent">
        <pre><code class="json">$$QUERY$$</code></pre>
      </div>
      <div class="tab-pane fade in" id="$$ID$$_received">
        <pre><code class="json">$$RESULT$$</code></pre>
      </div>
      <div class="tab-pane fade in" id="$$ID$$_validation">
        <pre><code class="json">$$VALIDATION$$</code></pre>
      </div>
    </div>
  </div>
</div>

