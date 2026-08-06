# #!/usr/bin/env python3
# import sys
# import time
# # raise Exception("This is a test error")

# sys.stdout.write("Content-Type: text/html\r\n")
# sys.stdout.write("\r\n")
# sys.stdout.write("<h1>CGI works!</h1>\r\n")
# sys.stdout.flush()
# time.sleep(30)


#!/usr/bin/env python3
import sys
import time
sys.stdout.write("Content-Type: text/html\r\n")
sys.stdout.write("\r\n")
sys.stdout.write("<h1>Slow CGI done!</h1>\r\n")
sys.stdout.flush()