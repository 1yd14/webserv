#!/usr/bin/php
<?php
header("Content-Type: text/html");
echo "Before error\n";
throw new Exception("This is a test PHP error");
echo "After error\n";
?>