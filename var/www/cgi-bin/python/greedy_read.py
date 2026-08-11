#!/usr/bin/env python3
import sys
# reads 1000 bytes but only 5 were sent
data = sys.stdin.read(1000)
print("Content-Type: text/plain")
print()
print(f"Read: {len(data)} bytes")