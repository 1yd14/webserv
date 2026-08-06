#!/bin/bash

WEBSERV=./webserv
TEST_DIR=config_files
PORT=8080
HOST=localhost

GREEN="\033[32m"
RED="\033[31m"
BLUE="\033[36m"
YELLOW="\033[33m"
RESET="\033[0m"

PASSED=0
FAILED=0
SERVER_PID=""
VALGRIND=false
if [ "$1" = "--valgrind" ] || [ "$1" = "-v" ]; then
    VALGRIND=true
fi

# Cleanup function
cleanup() {
    if [ -n "$SERVER_PID" ] && kill -0 "$SERVER_PID" 2>/dev/null; then
        kill "$SERVER_PID" 2>/dev/null
        wait "$SERVER_PID" 2>/dev/null
    fi
    # Kill any remaining webserv processes
    pkill -f "$WEBSERV" 2>/dev/null
    # Kill any remaining curl processes
    pkill -f curl 2>/dev/null
}

# Set trap for cleanup on script exit
trap cleanup EXIT INT TERM

# Function to wait for server to be ready
wait_for_server() {
    local max_attempts=30
    local attempt=0
    
    while [ $attempt -lt $max_attempts ]; do
        if curl -s -o /dev/null -w "%{http_code}" "http://${HOST}:${PORT}/" 2>/dev/null | grep -q "200\|301\|302\|400\|404"; then
            return 0
        fi
        sleep 0.1
        attempt=$((attempt + 1))
    done
    return 1
}

# Function to check if port is available
check_port() {
    if command -v lsof >/dev/null 2>&1; then
        if lsof -i ":$PORT" > /dev/null 2>&1; then
            echo -e "${RED}Port $PORT is already in use!${RESET}"
            return 1
        fi
    fi
    return 0
}

# Function to run HTTP tests using curl
run_http_test() {
    local description=$1
    local expected=$2
    local method=$3
    local url=$4
    local data=$5
    local extra_opts=$6
    
    # Build curl command
    local cmd="curl -s -o /dev/null -w '%{http_code}' --max-time 5"
    
    if [ -n "$method" ] && [ "$method" != "GET" ]; then
        cmd="$cmd -X $method"
    fi
    
    if [ -n "$data" ]; then
        cmd="$cmd -d '$data'"
    fi
    
    if [ -n "$extra_opts" ]; then
        cmd="$cmd $extra_opts"
    fi
    
    cmd="$cmd http://${HOST}:${PORT}$url"
    
    printf "Test %-50s : " "$description"
    
    local actual
    actual=$(eval "$cmd" 2>/dev/null)
    local ret=$?
    
    if [ $ret -ne 0 ]; then
        echo -e "${RED}FAILED (curl error: $ret)${RESET}"
        FAILED=$((FAILED+1))
        return
    fi
    
    if [ "$actual" = "$expected" ]; then
        echo -e "${GREEN}PASSED${RESET}"
        PASSED=$((PASSED+1))
    else
        echo -e "${RED}FAILED (expected $expected, got $actual)${RESET}"
        FAILED=$((FAILED+1))
    fi
}
run_http_test_with_file() {
    local description=$1
    local expected=$2
    local method=$3
    local url=$4
    local file=$5
    local extra_opts=$6
    
    printf "Test %-50s : " "$description"
    
    # Validate file exists
    if [ ! -f "$file" ]; then
        echo -e "${RED}FAILED (test file not found: $file)${RESET}"
        FAILED=$((FAILED+1))
        return
    fi
    
    # Get file size for logging
    local file_size=$(stat -c%s "$file" 2>/dev/null || stat -f%z "$file" 2>/dev/null)
    
    # Build curl command
    local cmd="curl -s -o /dev/null -w '%{http_code}' --max-time 10 -X $method"
    cmd="$cmd --data-binary @$file"
    
    if [ -n "$extra_opts" ]; then
        cmd="$cmd $extra_opts"
    fi
    
    cmd="$cmd http://${HOST}:${PORT}$url"
    
    # Execute and capture result
    local actual
    actual=$(eval "$cmd" 2>/dev/null)
    local ret=$?
    
    if [ $ret -ne 0 ]; then
        echo -e "${RED}FAILED (curl error: $ret, file size: ${file_size} bytes)${RESET}"
        FAILED=$((FAILED+1))
        return
    fi
    
    # Compare result
    if [ "$actual" = "$expected" ]; then
        echo -e "${GREEN}PASSED (${file_size} bytes)${RESET}"
        PASSED=$((PASSED+1))
    else
        echo -e "${RED}FAILED (expected $expected, got $actual, file size: ${file_size} bytes)${RESET}"
        FAILED=$((FAILED+1))
    fi
}


# Function to run raw TCP tests using netcat
run_raw_test() {
    local description=$1
    local expected=$2
    local request=$3
    
    printf "Test %-50s : " "$description"
    
    local actual
    actual=$(echo -e "$request" | timeout 2 nc "$HOST" "$PORT" 2>/dev/null | head -1 | awk '{print $2}')
    
    if [ "$actual" = "$expected" ]; then
        echo -e "${GREEN}PASSED${RESET}"
        PASSED=$((PASSED+1))
    else
        echo -e "${RED}FAILED (expected $expected, got $actual)${RESET}"
        FAILED=$((FAILED+1))
    fi
}

# Performance test: concurrent connections
run_concurrent_test() {
    local description=$1
    local num=$2
    local tmpdir
    
    printf "Test %-50s : " "$description"
    
    tmpdir=$(mktemp -d)
    local success=0
    local total=0
    
    # Create an array to store PIDs
    local pids=()
    
    # Launch all requests in background
    for i in $(seq 1 "$num"); do
        (
            curl -s -o /dev/null -w "%{http_code}" --max-time 2 "http://${HOST}:${PORT}/" 2>/dev/null > "$tmpdir/$i"
        ) &
        pids+=($!)
    done
    
    # Wait for all background processes
    for pid in "${pids[@]}"; do
        wait "$pid" 2>/dev/null
    done
    
    # Count results
    for i in $(seq 1 "$num"); do
        if [ -f "$tmpdir/$i" ]; then
            total=$((total + 1))
            code=$(cat "$tmpdir/$i" 2>/dev/null)
            if [ "$code" = "200" ]; then
                success=$((success + 1))
            fi
        fi
    done
    rm -rf "$tmpdir"
    
    if [ $total -eq 0 ]; then
        echo -e "${RED}FAILED (no responses received)${RESET}"
        FAILED=$((FAILED+1))
        return
    fi
    
    local rate=$((success * 100 / total))
    if [ $rate -ge 85 ]; then
        echo -e "${GREEN}PASSED (${success}/${total} = ${rate}%)${RESET}"
        PASSED=$((PASSED+1))
    else
        echo -e "${RED}FAILED (${success}/${total} = ${rate}%, need 85%)${RESET}"
        FAILED=$((FAILED+1))
    fi
}

# Performance test: rapid requests
run_rapid_test() {
    local description=$1
    local num=$2
    local success=0
    
    printf "Test %-50s : " "$description"
    
    local start
    start=$(date +%s%N)
    
    for i in $(seq 1 "$num"); do
        code=$(curl -s -o /dev/null -w "%{http_code}" --max-time 1 "http://${HOST}:${PORT}/" 2>/dev/null)
        if [ "$code" = "200" ]; then
            success=$((success + 1))
        fi
    done
    
    local end
    end=$(date +%s%N)
    
    local elapsed=$(( (end - start) / 1000000 ))  # ms
    if [ $elapsed -eq 0 ]; then
        elapsed=1
    fi
    local rps=$(( num * 1000 / elapsed ))
    local rate=$((success * 100 / num))
    
    if [ $rps -ge 50 ] && [ $rate -ge 85 ]; then
        echo -e "${GREEN}PASSED (${rps} req/s, ${rate}% success)${RESET}"
        PASSED=$((PASSED+1))
    else
        echo -e "${RED}FAILED (${rps} req/s, ${rate}% success, need 50 req/s and 85%)${RESET}"
        FAILED=$((FAILED+1))
    fi
}

# Test server stability after load
run_stability_test() {
    local description=$1
    printf "Test %-50s : " "$description"
    
    # Create an array for PIDs
    local pids=()
    
    # Send 100 requests in background
    for i in $(seq 1 100); do
        (
            curl -s -o /dev/null "http://${HOST}:${PORT}/" 2>/dev/null
        ) &
        pids+=($!)
    done
    
    # Wait for all background processes
    for pid in "${pids[@]}"; do
        wait "$pid" 2>/dev/null
    done
    
    # Check server is still responding
    local code
    code=$(curl -s -o /dev/null -w "%{http_code}" --max-time 2 "http://${HOST}:${PORT}/" 2>/dev/null)
    if [ "$code" = "200" ]; then
        echo -e "${GREEN}PASSED${RESET}"
        PASSED=$((PASSED+1))
    else
        echo -e "${RED}FAILED (server down after load)${RESET}"
        FAILED=$((FAILED+1))
    fi
}

# ============= Main Script =============

echo "============= Webserv Parser Testing ============="

# Check if webserv binary exists and is executable
if [ ! -f "$WEBSERV" ]; then
    echo -e "${YELLOW}Webserv binary not found. Building...${RESET}"
fi

# Step 0: Build the program
echo -e "${BLUE}Building webserv...${RESET}"
make re

if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed! Aborting tests.${RESET}"
    exit 1
fi
echo -e "${GREEN}Build succeeded.${RESET}"

# Check if test directory exists
if [ ! -d "$TEST_DIR" ]; then
    echo -e "${RED}Test directory '$TEST_DIR' not found!${RESET}"
    exit 1
fi

# Check if invalid directory exists
if [ ! -d "$TEST_DIR/invalid" ]; then
    echo -e "${YELLOW}Warning: Invalid config directory not found. Skipping invalid tests.${RESET}"
fi

# ============ Invalid Config Tests ============
echo ""
echo "============== Invalid config test =============="

if [ -d "$TEST_DIR/invalid" ]; then
    for file in "$TEST_DIR/invalid/"*; do
        if [ -f "$file" ]; then
            printf "Test %-40s : " "$(basename "$file")"
            OUTPUT=$($WEBSERV "$file" 2>&1)
            RET=$?
            
            if [ $RET -ne 0 ]; then
                echo -e "${GREEN}PASSED${RESET}"
                PASSED=$((PASSED+1))
            else
                echo -e "${RED}FAILED (exit $RET)${RESET}"
                FAILED=$((FAILED+1))
                echo -e "${BLUE}${OUTPUT}${RESET}"
            fi
        fi
    done
else
    echo -e "${YELLOW}Skipping invalid config tests (directory not found)${RESET}"
fi

# Check if port is available before starting server
if ! check_port; then
    echo -e "${RED}Cannot start server. Port $PORT is in use.${RESET}"
    exit 1
fi

# ============ Start Server for HTTP Tests ============
echo ""
echo -e "${BLUE}Starting webserver on port $PORT...${RESET}"

# Check if config file exists
if [ ! -f "$TEST_DIR/test_config.conf" ]; then
    echo -e "${RED}Config file '$TEST_DIR/test_config.conf' not found!${RESET}"
    exit 1
fi

if [ "$VALGRIND" = true ]; then
    valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes -s --log-file=valgrind_output.txt $WEBSERV "$TEST_DIR/test_config.conf" &
else
    $WEBSERV "$TEST_DIR/test_config.conf" &
fi
SERVER_PID=$!

# Wait for server to be ready
if ! wait_for_server; then
    echo -e "${RED}Server failed to start within timeout!${RESET}"
    kill "$SERVER_PID" 2>/dev/null
    exit 1
fi
echo -e "${GREEN}Server is ready.${RESET}"

# ============ HTTP Tests ============
echo ""
echo "============== HTTP Tests =============="

# ---- Basic GET ----
run_http_test "GET / returns 200" "200" "GET" "/"
run_http_test "GET /index.html returns 200" "200" "GET" "/index.html"
run_http_test "GET nonexistent returns 404" "404" "GET" "/nonexistent_file"
run_http_test "GET /files/ returns 200" "200" "GET" "/files/"
run_http_test "GET existing txt file returns 200" "200" "GET" "/files/a.txt"

# ---- Method validation ----
run_http_test "POST / with body returns 200" "200" "POST" "/" "hello"
run_http_test "POST / no body returns 411" "411" "POST" "/"
run_http_test "POST /files not allowed returns 405" "405" "POST" "/files" "hello"
run_http_test "DELETE / not allowed returns 405" "405" "DELETE" "/"
run_http_test "PATCH returns 405" "405" "PATCH" "/" "" "-H 'Content-Length: 0'"
run_http_test "PUT returns 405" "405" "PUT" "/" "" "-H 'Content-Length: 0'"
run_http_test "Invalid method lowercase returns 400" "400" "get" "/"
run_http_test "Unknown method BLABLA returns 405" "405" "BLABLA" "/"

# ---- Upload ----
run_http_test "POST /upload returns 201" "201" "POST" "/upload" "hello"
run_http_test "GET /upload not allowed returns 405" "405" "GET" "/upload"
run_http_test "DELETE uploaded file returns 204" "204" "DELETE" "/upload/upload"

# ---- Redirect ----
run_http_test "GET /old-page returns 301" "301" "GET" "/old-page"

# ---- HTTP version ----
run_http_test "HTTP/1.0 returns 200" "200" "GET" "/" "" "--http1.0"

# ---- CGI (if configured) ----
run_http_test "CGI Python returns 200" "200" "GET" "/cgi-bin/python/test.py"
run_http_test "CGI non-executable returns 403" "403" "GET" "/cgi-bin/python/noexec.py"

# ---- Header validation ----
run_http_test "Empty Host header returns 400" "400" "GET" "/" "" "-H 'Host:'"

# ---- Keep-alive ----
run_http_test "HTTP/1.1 keep-alive returns 200" "200" "GET" "/" "" "-H 'Connection: keep-alive'"
run_http_test "HTTP/1.1 connection close returns 200" "200" "GET" "/" "" "-H 'Connection: close'"

# ---- Error responses ----
# Create a small file for the body test instead of generating huge data
echo "Creating test file for body size test..."
dd if=/dev/zero of=/tmp/test_body.txt bs=1024 count=11000 2>/dev/null
run_http_test_with_file "Max body size exceeded returns 413" "413" "POST" "/" "/tmp/test_body.txt" "--max-time 3"
rm -f /tmp/test_body.txt

run_http_test "404 returns custom error page" "404" "GET" "/doesnotexist"
run_http_test "405 not allowed" "405" "DELETE" "/"

# ---- URI validation ----
run_http_test "Valid URL encoded returns 200" "200" "GET" "/index.html"

# ============ Raw Request Tests ============
echo ""
echo "============== Raw Request Tests =============="

run_raw_test "Directory traversal returns 400" "400" "GET /../etc/passwd HTTP/1.1\r\nHost: localhost\r\n\r\n"
run_raw_test "Empty request returns 400" "400" "\r\n\r\n"
run_raw_test "Missing HTTP version returns 400" "400" "GET /\r\nHost: localhost\r\n\r\n"
run_raw_test "HTTP/2.0 returns 505" "505" "GET / HTTP/2.0\r\nHost: localhost\r\n\r\n"
run_raw_test "HTTP/0.9 returns 505" "505" "GET / HTTP/0.9\r\nHost: localhost\r\n\r\n"
run_raw_test "Double colon in header returns 400" "400" "GET / HTTP/1.1\r\nHost:: localhost\r\n\r\n"
run_raw_test "Space before colon in header returns 400" "400" "GET / HTTP/1.1\r\nHost : localhost\r\n\r\n"
run_raw_test "Folded header returns 400" "400" "GET / HTTP/1.1\r\nHost: localhost\r\nX-Test: value\r\n continued\r\n\r\n"
run_raw_test "Missing Host HTTP/1.1 returns 400" "400" "GET / HTTP/1.1\r\n\r\n"
run_raw_test "GET with valid path returns 200" "200" "GET / HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n"
run_raw_test "HEAD request returns 405" "405" "HEAD / HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n"
run_raw_test "Negative Content-Length returns 400" "400" "GET / HTTP/1.1\r\nHost: localhost\r\nContent-Length: -1\r\n\r\n"
run_raw_test "Non-digit Content-Length returns 400" "400" "GET / HTTP/1.1\r\nHost: localhost\r\nContent-Length: abc\r\n\r\n"


# ============ Additional Edge Cases ============
echo ""
echo "============== Additional Edge Cases =============="

# Query string
run_http_test "GET with query string returns 200" "200" "GET" "/index.html?foo=bar&baz=123"

# Percent-encoded path
run_http_test "GET with percent-encoded path returns 200" "200" "GET" "/files/hello%20world.txt"

# Trailing slash
run_http_test "GET /files (no slash) returns 200" "200" "GET" "/files"

# Very long URI (8k)
printf "Test %-50s : " "Very long URI (8k) returns 414/400"
long_uri="/$(printf 'a%.0s' {1..8000})"
code=$(curl -s -o /dev/null -w "%{http_code}" --max-time 5 "http://${HOST}:${PORT}${long_uri}" 2>/dev/null)
if [ "$code" = "414" ] || [ "$code" = "400" ]; then
    echo -e "${GREEN}PASSED (got $code)${RESET}"
    PASSED=$((PASSED+1))
else
    echo -e "${RED}FAILED (expected 414 or 400, got $code)${RESET}"
    FAILED=$((FAILED+1))
fi

# Null byte in URI
run_raw_test "Null byte in URI returns 400" "400" "GET /index.html%00test HTTP/1.1\r\nHost: localhost\r\n\r\n"

# Duplicate Host header
run_raw_test "Duplicate Host header returns 400" "400" "GET / HTTP/1.1\r\nHost: localhost\r\nHost: other\r\n\r\n"

# Host with port
run_http_test "Host header with port returns 200" "200" "GET" "/" "" "-H 'Host: localhost:8080'"

# HTTP/1.0 without Host (should be allowed)
run_raw_test "HTTP/1.0 without Host returns 200" "200" "GET / HTTP/1.0\r\n\r\n"

# Multiple Content-Length
run_raw_test "Multiple Content-Length returns 400" "400" "POST / HTTP/1.1\r\nHost: localhost\r\nContent-Length: 5\r\nContent-Length: 10\r\n\r\nhello"

# Content-Length + Transfer-Encoding
run_raw_test "Content-Length and Transfer-Encoding conflict" "400" "POST / HTTP/1.1\r\nHost: localhost\r\nContent-Length: 5\r\nTransfer-Encoding: chunked\r\n\r\n5\r\nhello\r\n0\r\n\r\n"
# Oversized header (16k)
printf "Test %-50s : " "Oversized header (16k) returns 431/400"
big_header="X-Big: $(printf 'a%.0s' {1..16000})"
code=$(curl -s -o /dev/null -w "%{http_code}" --max-time 6 -H "$big_header" "http://${HOST}:${PORT}/" 2>/dev/null)
if [ "$code" = "431" ] || [ "$code" = "400" ]; then
    echo -e "${GREEN}PASSED (got $code)${RESET}"
    PASSED=$((PASSED+1))
else
    echo -e "${RED}FAILED (expected 431 or 400, got $code)${RESET}"
    FAILED=$((FAILED+1))
fi

# Header with underscore
run_raw_test "Header with underscore returns 200" "200" "GET / HTTP/1.1\r\nHost: localhost\r\nX_Custom: value\r\n\r\n"

# Header with non-ASCII
run_raw_test "Header with non-ASCII returns 400" "400" "GET / HTTP/1.1\r\nHost: localhost\r\nX-Test: café\r\n\r\n"

# GET with body (should be ignored or 400)
run_http_test "GET with body returns 200" "200" "GET" "/" "body-data"

# DELETE with body
# Create file then delete it
printf "Test %-50s : " "DELETE uploaded file returns 204"
curl -s -o /dev/null -X POST -H "Content-Length: 5" -d "hello" "http://localhost:8080/upload/testdelete.txt" 2>/dev/null
code=$(curl -s -o /dev/null -w "%{http_code}" -X DELETE "http://localhost:8080/upload/testdelete.txt" 2>/dev/null)
if [ "$code" = "204" ]; then
    echo -e "${GREEN}PASSED${RESET}"
    PASSED=$((PASSED+1))
else
    echo -e "${RED}FAILED (expected 204, got $code)${RESET}"
    FAILED=$((FAILED+1))
fi
# POST to directory
run_http_test "POST to directory returns 405" "405" "POST" "/files/" "hello"

# HEAD method (if supported)
run_raw_test "HEAD request returns 405" "405" "HEAD / HTTP/1.1\r\nHost: localhost\r\n\r\n"

# ============ Response Header Checks ============
echo ""
echo "============== Response Header Checks =============="


# POST to directory
run_http_test "POST to directory returns 405" "405" "POST" "/files/" "hello"

# HEAD method (if supported)
run_raw_test "HEAD request returns 405" "405" "HEAD / HTTP/1.1\r\nHost: localhost\r\n\r\n"

# ============ Response Header Checks ============
echo ""
echo "============== Response Header Checks =============="

check_response_header() {
    local description=$1
    local url=$2
    local header=$3
    local expected_pattern=$4
    
    printf "Test %-50s : " "$description"
   local header_value=$(curl -s -D - -o /dev/null "http://${HOST}:${PORT}$url" 2>/dev/null | grep -i "^$header:" | head -1)
    if echo "$header_value" | grep -q "$expected_pattern"; then
        echo -e "${GREEN}PASSED${RESET}"
        PASSED=$((PASSED+1))
    else
        echo -e "${RED}FAILED (expected header '$header' matching '$expected_pattern', got '$header_value')${RESET}"
        FAILED=$((FAILED+1))
    fi
}

check_response_header "Server header present" "/" "Server" "webserv"
check_response_header "Content-Type for HTML" "/index.html" "Content-Type" "text/html"
check_response_header "Content-Type for text" "/files/a.txt" "Content-Type" "text/plain"
check_response_header "Content-Length present" "/" "Content-Length" "[0-9]"
check_response_header "Connection: keep-alive" "/" "Connection" "keep-alive"

# ============ Additional Stress ============
echo ""
echo "============== Additional Stress Tests =============="

# Large file serve
printf "Test %-50s : " "Serve large file (10MB) without crash"
dd if=/dev/zero of="var/www/html/files/large.bin" bs=1M count=10 2>/dev/null
code=$(curl -s -o /dev/null -w "%{http_code}" --max-time 10 "http://${HOST}:${PORT}/files/large.bin" 2>/dev/null)
if [ "$code" = "200" ]; then
    echo -e "${GREEN}PASSED${RESET}"
    PASSED=$((PASSED+1))
else
    echo -e "${RED}FAILED (got $code)${RESET}"
    FAILED=$((FAILED+1))
fi
rm -f "var/www/html/files/large.bin"

# Slow client
printf "Test %-50s : " "Slow client (partial request) handled gracefully"
(echo -n "GET / HTTP/1.1\r\n"; sleep 0.5; echo -n "Host: localhost\r\n"; sleep 0.5; echo -e "\r\n") | timeout 3 nc "$HOST" "$PORT" > /dev/null 2>&1
code=$(curl -s -o /dev/null -w "%{http_code}" --max-time 2 "http://${HOST}:${PORT}/" 2>/dev/null)
if [ "$code" = "200" ]; then
    echo -e "${GREEN}PASSED${RESET}"
    PASSED=$((PASSED+1))
else
    echo -e "${RED}FAILED (server not responding after slow client)${RESET}"
    FAILED=$((FAILED+1))
fi

# Keep-alive multiple requests on same connection
printf "Test %-50s : " "Keep-alive: multiple requests"
count=$(curl -s -o /dev/null -w "%{http_code}\n" --keepalive "http://localhost:8080/" "http://localhost:8080/" "http://localhost:8080/" | grep -c "200")
if [ "$count" -ge 3 ]; then
    echo -e "${GREEN}PASSED${RESET}"
    PASSED=$((PASSED+1))
else
    echo -e "${RED}FAILED (expected 3 responses, got $count)${RESET}"
    FAILED=$((FAILED+1))
fi
# ============ Configuration Tests ============
echo ""
echo "============== Configuration Tests =============="

# Custom 404 page content
printf "Test %-50s : " "404 custom page content"
content=$(curl -s "http://${HOST}:${PORT}/doesnotexist" 2>/dev/null)
if echo "$content" | grep -q "404" || echo "$content" | grep -q "Not Found"; then
    echo -e "${GREEN}PASSED${RESET}"
    PASSED=$((PASSED+1))
else
    echo -e "${RED}FAILED (custom 404 page not served)${RESET}"
    FAILED=$((FAILED+1))
fi

# Index file served
printf "Test %-50s : " "Directory index served"
content=$(curl -s "http://${HOST}:${PORT}/" 2>/dev/null)
if echo "$content" | grep -q "index" || echo "$content" | grep -q "html"; then
    echo -e "${GREEN}PASSED${RESET}"
    PASSED=$((PASSED+1))
else
    echo -e "${RED}FAILED (index not served)${RESET}"
    FAILED=$((FAILED+1))
fi
# ============ Performance Tests ============
echo ""
echo "============== Performance Tests =============="

run_concurrent_test "Concurrent connections (10)" 10
run_rapid_test "Rapid requests (50)" 50
run_stability_test "Server stability after load"
run_http_test "Server responds after stress" "200" "GET" "/"

# =============================================================================
# ADVANCED EDGE CASES & SECURITY TESTS
# =============================================================================

echo ""
echo "============== Advanced Security & Edge Cases =============="

# ---- Directory traversal variants ----
run_raw_test "Path traversal with %2E%2E encoding" "400" "GET /%2E%2E/%2E%2E/etc/passwd HTTP/1.1\r\nHost: localhost\r\n\r\n"
run_raw_test "Path traversal with double encoding %252E" "400" "GET /%252E%252E/%252E%252E/etc/passwd HTTP/1.1\r\nHost: localhost\r\n\r\n"
run_raw_test "Path traversal with Unicode / (U+2044)" "400" "GET /⁄⁄etc/passwd HTTP/1.1\r\nHost: localhost\r\n\r\n"  # note: ⁄ is U+2044 (fraction slash), might be normalized
run_raw_test "Path traversal with backslash" "400" "GET /..\\..\\etc\\passwd HTTP/1.1\r\nHost: localhost\r\n\r\n"

# ---- URI with multiple slashes (should be normalized) ----
run_http_test "Multiple slashes in URI" "200" "GET" "///index.html"

# ---- URI with encoded spaces in path ----
run_http_test "Path with + as space" "200" "GET" "/files/hello+world.txt"   # if you have a file "hello world.txt"

# ---- Absolute URI (RFC) ----
run_raw_test "Absolute URI in request line" "200" "GET http://localhost:8080/ HTTP/1.1\r\nHost: localhost\r\n\r\n"

# ---- Invalid request line ----
run_raw_test "Missing space in request line" "400" "GET/ HTTP/1.1\r\nHost: localhost\r\n\r\n"
run_raw_test "Extra space after method" "400" "GET  / HTTP/1.1\r\nHost: localhost\r\n\r\n"
run_raw_test "Method with non-ASCII" "400" "GÉT / HTTP/1.1\r\nHost: localhost\r\n\r\n"

# ---- HTTP version with leading zeros ----
run_raw_test "HTTP/1.01 returns 400" "400" "GET / HTTP/1.01\r\nHost: localhost\r\n\r\n"
run_raw_test "HTTP/1.1.1 returns 400" "400" "GET / HTTP/1.1.1\r\nHost: localhost\r\n\r\n"

# ---- Header with trailing whitespace ----
run_raw_test "Header value with trailing space" "200" "GET / HTTP/1.1\r\nHost: localhost \r\n\r\n"

# ---- Header name with colon in value (should be accepted) ----
run_raw_test "Header value with colon" "200" "GET / HTTP/1.1\r\nHost: localhost\r\nX-Test: a:b\r\n\r\n"

# ---- Request with no headers (only request line) ----
run_raw_test "Request line only" "400" "GET / HTTP/1.1\r\n\r\n"

# ---- Very small request (just \r\n) ----
run_raw_test "Just CRLF" "400" "\r\n\r\n"

# ---- Extra data after request ----
# This is tricky; we can send a request with extra bytes after body
printf "Test %-50s : " "Extra data after request (garbage)"
output=$(echo -e "GET / HTTP/1.1\r\nHost: localhost\r\n\r\nGARBAGE" | timeout 2 nc "$HOST" "$PORT" 2>/dev/null | head -1)
code=$(echo "$output" | awk '{print $2}')
if [ "$code" = "200" ] || [ -z "$code" ]; then
    echo -e "${GREEN}PASSED (ignored or 200)${RESET}"
    PASSED=$((PASSED+1))
else
    echo -e "${RED}FAILED (expected 200 or ignored, got $code)${RESET}"
    FAILED=$((FAILED+1))
fi

# =============================================================================
# CHUNKED TRANSFER ENCODING (if supported)
# =============================================================================
echo ""
echo "============== Chunked Transfer Encoding Tests =============="

# For chunked tests, we need to craft raw requests with chunked body.
# We'll use netcat or printf.

test_chunked() {
    local desc=$1
    local expected=$2
    local chunks=$3
    printf "Test %-50s : " "$desc"
    local response=$(printf "%b" "$chunks" | timeout 2 nc "$HOST" "$PORT" 2>/dev/null)
    local code=$(echo "$response" | head -1 | awk '{print $2}')
    if [ "$code" = "$expected" ]; then
        echo -e "${GREEN}PASSED${RESET}"
        PASSED=$((PASSED+1))
    else
        echo -e "${RED}FAILED (expected $expected, got $code)${RESET}"
        FAILED=$((FAILED+1))
    fi
}

# Valid chunked request
chunked_valid="POST /upload HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n5\r\nhello\r\n6\r\n world\r\n0\r\n\r\n"
test_chunked "Valid chunked POST" "201" "$chunked_valid"

# Chunked with missing final 0
chunked_no_final="POST /upload HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n5\r\nhello\r\n"
test_chunked "Chunked missing final chunk" "" "$chunked_no_final"

# Chunked with invalid chunk size (non-hex)
chunked_invalid_hex="POST /upload HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\nX\r\nhello\r\n0\r\n\r\n"
test_chunked "Chunked invalid hex size" "400" "$chunked_invalid_hex"


# Chunked with both Content-Length and TE (conflict)
chunked_conflict="POST /upload HTTP/1.1\r\nHost: localhost\r\nContent-Length: 5\r\nTransfer-Encoding: chunked\r\n\r\n5\r\nhello\r\n0\r\n\r\n"
test_chunked "Chunked + Content-Length conflict" "400" "$chunked_conflict"

# =============================================================================
# CGI ADVANCED TESTS
# =============================================================================
echo ""
echo "============== CGI Advanced Tests =============="

# Test CGI with query string and POST data (if your script handles it)
run_http_test "CGI GET with query string" "200" "GET" "/cgi-bin/python/test.py?name=value"
run_http_test "CGI POST with data" "200" "POST" "/cgi-bin/python/test.py" "data=hello"

# Test CGI with missing interpreter (should return 502/500)
# Create a script with shebang #!/nonexistent
# For this test you need to create a file cgi-bin/python/bad_interpreter.py with that shebang and make it executable
# Then test:
# run_http_test "CGI with invalid interpreter returns 502" "502" "GET" "/cgi-bin/python/bad_interpreter.py"

# Test CGI that returns a custom status (e.g., script that outputs "Status: 404")
# Create a script that prints "Status: 404 Not Found" before headers
# Then test.

# Test CGI timeout (if server has timeout)
# Create a script that sleeps for 5 seconds, then test with short curl timeout
# But we can only test if server times out, not curl.

# =============================================================================
# STRESS & PERFORMANCE (HEAVY)
# =============================================================================
echo ""
echo "============== Heavy Stress Tests =============="

# Increased concurrency
run_concurrent_test "Concurrent connections (50)" 50

# Rapid requests with more iterations
run_rapid_test "Rapid requests (200)" 200

# Extended stability: 500 requests in background
printf "Test %-50s : " "Stability after 500 requests"
pids=()
for i in $(seq 1 500); do
    curl -s -o /dev/null "http://${HOST}:${PORT}/" 2>/dev/null &
    pids+=($!)
    # Throttle to avoid fork bomb
    if [ $((i % 50)) -eq 0 ]; then
        sleep 0.1
    fi
done
for pid in "${pids[@]}"; do
    wait "$pid" 2>/dev/null
done
code=$(curl -s -o /dev/null -w "%{http_code}" --max-time 2 "http://${HOST}:${PORT}/" 2>/dev/null)
if [ "$code" = "200" ]; then
    echo -e "${GREEN}PASSED${RESET}"
    PASSED=$((PASSED+1))
else
    echo -e "${RED}FAILED (server down)${RESET}"
    FAILED=$((FAILED+1))
fi

# Memory leak test: 1000 requests and check if server still responds
printf "Test %-50s : " "Memory leak (1000 requests)"
for i in $(seq 1 1000); do
    curl -s -o /dev/null "http://${HOST}:${PORT}/" 2>/dev/null
    if [ $((i % 200)) -eq 0 ]; then
        # Quick check server still alive
        if ! curl -s -o /dev/null -w "%{http_code}" --max-time 1 "http://${HOST}:${PORT}/" 2>/dev/null | grep -q "200"; then
            echo -e "${RED}FAILED (server crashed at request $i)${RESET}"
            FAILED=$((FAILED+1))
            break
        fi
    fi
done
# If loop completed
if [ $i -ge 1000 ]; then
    echo -e "${GREEN}PASSED (server survived)${RESET}"
    PASSED=$((PASSED+1))
fi

# =============================================================================
# AUTOINDEX / DIRECTORY LISTING
# =============================================================================
echo ""
echo "============== Directory Listing Tests =============="

# If your server supports autoindex, test it (but 42 subject might not require)
# We'll test if /files/ returns a listing (200) or 403/404.
# If you have a directory with index.html, it will return that.
# We'll just check status.
run_http_test "Directory /files/ listing" "200" "GET" "/files/"

# Check if listing contains some file names (if enabled)
printf "Test %-50s : " "Directory listing contains expected files"
content=$(curl -s "http://${HOST}:${PORT}/files/" 2>/dev/null)
if echo "$content" | grep -q "a.txt" || echo "$content" | grep -q "hello"; then
    echo -e "${GREEN}PASSED${RESET}"
    PASSED=$((PASSED+1))
else
    echo -e "${RED}FAILED (no listing or missing files)${RESET}"
    FAILED=$((FAILED+1))
fi


# =============================================================================
# REQUEST WITH EXTREMELY MANY HEADERS
# =============================================================================
echo ""
echo "============== Many Headers Test =============="

printf "Test %-50s : " "100 custom headers"
headers=""
for i in {1..97}; do
    headers="$headers -H 'X-Header-$i: value'"
done
cmd="curl -s -o /dev/null -w '%{http_code}' $headers http://${HOST}:${PORT}/"
code=$(eval "$cmd" 2>/dev/null)
if [ "$code" = "200" ]; then
    echo -e "${GREEN}PASSED${RESET}"
    PASSED=$((PASSED+1))
else
    echo -e "${RED}FAILED (got $code)${RESET}"
    FAILED=$((FAILED+1))
fi




# ============ Test Summary ============
echo ""
echo "---- Test summary ----"
echo -e "PASSED TESTS: ${GREEN}${PASSED}${RESET}"
echo -e "FAILED TESTS: ${RED}${FAILED}${RESET}"
TOTAL=$((PASSED + FAILED))
if [ $TOTAL -gt 0 ]; then
    RATE=$((PASSED * 100 / TOTAL))
    echo -e "SUCCESS RATE: ${GREEN}${RATE}%${RESET}"
fi
echo ""
echo "============= End of Testing ============="

# Cleanup is handled by trap
exit 0