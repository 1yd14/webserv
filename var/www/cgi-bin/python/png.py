#!/usr/bin/env python3
import sys
import base64

# 1x1 red pixel PNG
png = base64.b64decode("iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mP8z8BQDwADhQGAWjR9awAAAABJRU5ErkJggg==")
sys.stdout.buffer.write(b"Content-Type: image/png\n\n")
sys.stdout.buffer.write(png)