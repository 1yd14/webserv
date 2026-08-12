#!/usr/bin/env python3
import os

print("Content-Type: text/plain")
print()

# Try to open a file with no permission
try:
    with open("/etc/shadow", "r") as f:
        print(f.read())
except PermissionError as e:
    print(f"Permission denied: {e}")

# Try to open a file that doesn't exist
try:
    with open("/nonexistent/file.txt", "r") as f:
        print(f.read())
except FileNotFoundError as e:
    print(f"File not found: {e}")