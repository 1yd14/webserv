#!/usr/bin/env python3
import os
import sys

print("Content-Type: text/html; charset=UTF-8\r\n\r")

body = ""
try:
    length = int(os.environ.get("CONTENT_LENGTH", 0) or 0)
    if length > 0:
        body = sys.stdin.read(length)
except (ValueError, TypeError):
    pass

print("""<!DOCTYPE html>
<html><head><title>webserv CGI test</title></head>
<body style="font-family: monospace; background:#111; color:#0af; padding:2em;">
<h1>🐍 Python CGI is alive</h1>
""")

print("<h2>Request info</h2><pre>")
for key in ["REQUEST_METHOD", "SCRIPT_NAME", "QUERY_STRING", "PATH_INFO",
            "CONTENT_TYPE", "CONTENT_LENGTH", "SERVER_NAME", "SERVER_PORT",
            "SERVER_PROTOCOL", "GATEWAY_INTERFACE"]:
    print(f"{key}: {os.environ.get(key, '(missing)')}")
print("</pre>")

print("<h2>All HTTP_* headers received</h2><pre>")
for key, val in sorted(os.environ.items()):
    if key.startswith("HTTP_"):
        print(f"{key} = {val}")
print("</pre>")

print("<h2>POST body (if any)</h2><pre>")
print(body if body else "(empty)")
print("</pre>")

print("</body></html>")