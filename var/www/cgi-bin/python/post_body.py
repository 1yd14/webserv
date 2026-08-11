#!/usr/bin/env python3
import sys
import os

body = sys.stdin.read(int(os.environ.get("CONTENT_LENGTH", 0)))
print("Content-Type: text/plain")
print()
print(f"Received: {body}")