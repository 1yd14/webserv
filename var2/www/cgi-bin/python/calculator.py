#!/usr/bin/env python3

import os
import urllib.parse
import html
import sys

sys.stdout.write("Content-Type: text/html\r\n")
sys.stdout.write("\r\n")

class Calculator:
    def __init__(self):
        self.a = 0
        self.b = 0

    def check_operation(self, operator):
        if operator == "+":
            return self.a + self.b
        elif operator == "-":
            return self.a - self.b
        elif operator == "*":
            return self.a * self.b
        elif operator == "/":
            if self.b == 0:
                return "Error: Division by zero"
            return self.a / self.b
        else:
            return "Sorry, no command found"

    def get_result(self, a, b, operator):
        self.a = a
        self.b = b
        return self.check_operation(operator)


def get_post_data():
    length = int(os.environ.get("CONTENT_LENGTH", 0))
    body = os.read(0, length).decode("utf-8")
    return urllib.parse.parse_qs(body)


data = get_post_data()

result = ""

if "submit" in data:
    try:
        n1 = float(data.get("n1", ["0"])[0])
        n2 = float(data.get("n2", ["0"])[0])
        op = data.get("op", ["+"])[0]

        calc = Calculator()
        result = calc.get_result(n1, n2, op)

    except Exception as e:
        result = "Error: " + str(e)


print(f"""
<!DOCTYPE html>
<html>
<head>
<title>Python CGI Calculator</title>
</head>

<body>
<form method="post">

<table align="center">

<tr>
<td><strong>{html.escape(str(result))}</strong></td>
</tr>

<tr>
<td>Enter 1st Number</td>
<td><input type="text" name="n1"></td>
</tr>

<tr>
<td>Enter 2nd Number</td>
<td><input type="text" name="n2"></td>
</tr>

<tr>
<td>Select Operator</td>
<td>
<select name="op">
<option value="+">+</option>
<option value="-">-</option>
<option value="*">*</option>
<option value="/">/</option>
</select>
</td>
</tr>

<tr>
<td></td>
<td>
<input type="submit" name="submit" value=" = ">
</td>
</tr>

</table>

</form>
</body>
</html>
""")