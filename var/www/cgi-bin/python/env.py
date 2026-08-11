#!/usr/bin/env python3
import os

print("Content-Type: text/plain")
print()
print(f"METHOD: {os.environ.get('REQUEST_METHOD', 'not set')}")
print(f"QUERY: {os.environ.get('QUERY_STRING', 'not set')}")
print(f"CONTENT_LENGTH: {os.environ.get('CONTENT_LENGTH', 'not set')}")