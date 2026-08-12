*This project has been created as part of the 42 curriculum by rmhazres, lyvan-de.*

# webserv

## Description

webserv is a from-scratch HTTP/1.1 compliant web server written in C++20. The goal is to build a fully functional web server — without using any external HTTP libraries — that can serve static files, handle CGI scripts, process file uploads, and manage multiple virtual servers simultaneously.

The server is built around a single-threaded, non-blocking I/O model using Linux's `epoll` API, closely mirroring how production servers like NGINX handle high concurrency without threads.

**Key features:**
- HTTP/1.1 compliant (GET, POST, DELETE)
- Non-blocking I/O via `epoll`
- Multiple virtual servers on multiple ports and IPs
- Static file serving with MIME type detection
- Directory listing (autoindex)
- File uploads and deletes
- CGI execution (Python, PHP) with non-blocking pipe I/O
- Chunked transfer encoding (send and receive)
- Multipart form-data parsing for browser file uploads
- Persistent connections (keep-alive)
- Custom error pages
- Redirects
- Configuration file parsing (NGINX-inspired syntax)
- URL decoding and query string handling
- Request validation (URI length, header size, body size, method validation)
- Comprehensive security checks (path traversal, null bytes, encoded attacks)

## Instructions

### Requirements

- Linux (Ubuntu 20.04+ recommended)
- C++20 compiler (g++ or clang++)
- GNU Make

### Compilation

```bash
make
```

To clean object files:
```bash
make clean
```

To fully rebuild:
```bash
make fclean && make
```

### Running the server

```bash
./webserv <config_file>
```

Example:
```bash
./webserv config_files/test_config.conf
```

### Configuration file

The configuration file uses an NGINX-inspired syntax. Example:

```nginx
server {
    listen          8080;
    host            127.0.0.1;
    root            /var/www/html;
    index           index.html;
    max_body_size   10M;

    error_page 404 /var/www/errors/404.html;
    error_page 500 /var/www/errors/500.html;

    location / {
        methods     GET POST;
        autoindex   off;
        index       index.html;
    }

    location /upload {
        methods     POST DELETE;
        upload_dir  /var/www/uploads;
    }

    location /cgi-bin/python {
        methods         GET POST;
        cgi_extension   .py;
        root            /var/www/cgi-bin/python;
    }

    location /old-page {
        redirect    301 /new-page;
    }
}
```

### Testing

Run the test suite:
```bash
./config_files/tester_config.sh
```

Run with valgrind memory check:
```bash
./config_files/tester_config.sh --valgrind
```

Run load test with siege:
```bash
siege -c 200 -r 50 http://localhost:8080/
```

### Project structure

```
webserv/
└── config_files/
    ├── config.conf       # A general config file
    └── invalid/          # Invalid config test cases
├── src/
│   ├── Common/           # Shared types: HttpRequest, HttpResponse, HttpStatus, Utils
│   ├── Config/           # Config file parser, Server and LocationBlock structs
│   ├── Connection/       # Connection for every request, main event loop
│   ├── Http/             # HTTP parsing, validation, routing, response building, CGI
│   ├── ServerSetup/      # Listening socket setup
│   ├── Signals/          # Signal handeling 
│   └── main.cpp          # Main entrypoint
├── var/www/              # Content for the firt website
│   ├── cgi-bin/          # CGI scripts (Python, PHP)
│   └── errors/           # Custom error pages
│   ├── html/             # Document root
│   ├── uploads/          # Upload directory
├── var2/www/             # Content for the second website
│   ├── cgi-bin/          # CGI scripts (Python, PHP)
│   └── errors/           # Custom error pages
│   ├── html/             # Document root
│   ├── uploads/          # Upload directory
├── Makefile			  # Makes the project
├── README.md			  # This README
├── tester.sh             # General tester
```

## Resources

### RFC and HTTP specification
- [RFC 7230 – HTTP/1.1 Message Syntax and Routing](https://datatracker.ietf.org/doc/html/rfc7230)
- [RFC 7231 – HTTP/1.1 Semantics and Content](https://datatracker.ietf.org/doc/html/rfc7231)
- [RFC 3875 – The Common Gateway Interface (CGI/1.1)](https://datatracker.ietf.org/doc/html/rfc3875)
- [MDN Web Docs – HTTP](https://developer.mozilla.org/en-US/docs/Web/HTTP)

### epoll and non-blocking I/O
- [epoll(7) Linux man page](https://man7.org/linux/man-pages/man7/epoll.7.html)
- [Single threaded non bocking IO model in NodeJS](https://www.geeksforgeeks.org/node-js/how-the-single-threaded-non-blocking-io-model-works-in-nodejs/)

### Testing tools
- [siege](https://www.joedog.org/siege-home/) – HTTP load testing
- [curl](https://curl.se/docs/) – HTTP client for manual testing

### AI usage

Claude (Anthropic) was used throughout this project as a guided learning tool and debugging aid:

- **Debugging:** Analysing valgrind output, epoll event traces, and HTTP protocol edge cases. Claude was used to reason through bugs without providing direct code answers, keeping the learning process intact.
- **HTTP protocol understanding:** Clarifying RFC requirements for headers, chunked encoding, CGI environment variables, and multipart form data.
- **Architecture guidance:** Discussing the state machine design for connections (`READING`, `WRITING`, `AWAITING_CGI`, `ERROR_PENDING`) and separation of concerns between parser, validator, router, and response builder.
- **Test design:** Designing edge case tests for security (path traversal, null bytes, oversized headers), correctness (pipelining, keep-alive, chunked encoding), and performance (siege load testing, valgrind leak checks).
- **Code review:** Reviewing pull requests for logic errors, dangling references, and protocol compliance issues.

All implementation decisions, debugging steps, and code were written by the project authors. Claude acted as a knowledgeable reference and sounding board, never as a code generator.
