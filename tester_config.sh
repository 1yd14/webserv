#!/bin/bash

WEBSERV=./webserv
TEST_DIR=config_files

GREEN="\033[32m"
RED="\033[31m"
BLUE="\033[36m"
RESET="\033[0m"

PASSED=0
FAILED=0

echo "============= Webserv Parser Testing ============="

# --------- STEP 0: build the program ---------
echo -e "${BLUE}Building webserv...${RESET}"
make re

# Check if make succeeded
if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed! Aborting tests.${RESET}"
    exit 1
fi
echo -e "${GREEN}Build succeeded.${RESET}"

# ----------------------------
# Test invalid scenes (expected: exit 1)
# ----------------------------

echo ""
echo "============== Invalid config test =============="

for file in "$TEST_DIR/invalid/"*; do
	printf "Test %-40s : " "$(basename "$file")"
	$WEBSERV "$file" > /dev/null 2>&1
	OUTPUT=$($WEBSERV "$file")
	RET=$?

	if [ $RET -ne 0 ]; then
		echo -e "${GREEN}PASSED${RESET}"
		PASSED=$((PASSED+1))
	else
		echo -e "${RED}FAILED (exit $RET)${RESET}"
		FAILED=$((FAILED+1))
	fi
	echo -e "${BLUE}${OUTPUT}${RESET}"
done

echo ""
echo "---- Test summary ----"
echo -e "PASSED TESTS: ${GREEN}${PASSED}${RESET}"
echo -e "FAILED TESTS: ${RED}${FAILED}${RESET}"
echo ""
echo "============= End of Parser Testing ============="