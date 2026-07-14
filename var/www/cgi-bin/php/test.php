#!/usr/bin/php-cgi
<?php
echo "Content-Type: text/html\r\n\r\n";
?>
<!DOCTYPE html>
<html>
<head><title>webserv CGI test</title></head>
<body style="font-family: monospace; background:#111; color:#0f0; padding:2em;">
<h1>🐘 PHP CGI is alive</h1>

<h2>Request info</h2>
<pre><?php
echo "Method:       " . $_SERVER['REQUEST_METHOD'] . "\n";
echo "Script name:  " . $_SERVER['SCRIPT_NAME'] . "\n";
echo "Query string: " . $_SERVER['QUERY_STRING'] . "\n";
echo "Path info:    " . ($_SERVER['PATH_INFO'] ?? '(empty)') . "\n";
echo "Content type: " . ($_SERVER['CONTENT_TYPE'] ?? '(none)') . "\n";
echo "Content len:  " . ($_SERVER['CONTENT_LENGTH'] ?? '(none)') . "\n";
echo "Server:       " . $_SERVER['SERVER_NAME'] . ":" . $_SERVER['SERVER_PORT'] . "\n";
echo "Protocol:     " . $_SERVER['SERVER_PROTOCOL'] . "\n";
?></pre>

<h2>All HTTP_* headers received</h2>
<pre><?php
foreach ($_SERVER as $key => $val) {
    if (strpos($key, 'HTTP_') === 0) {
        echo "$key = $val\n";
    }
}
?></pre>

<h2>GET params</h2>
<pre><?php print_r($_GET); ?></pre>

<h2>POST body (if any)</h2>
<pre><?php
$body = file_get_contents('php://stdin');
echo htmlspecialchars($body ?: '(empty)');
?></pre>

</body>
</html>