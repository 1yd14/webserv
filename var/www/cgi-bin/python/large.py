#!/usr/bin/env python3

print("Content-Type: text/plain")
print()

for i in range(100000):
    print(f"Line {i}: " + "A" * 100)
print("\r\n")