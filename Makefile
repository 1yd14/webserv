# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/07 11:50:34 by lyvan-de          #+#    #+#              #
#    Updated: 2026/05/21 10:50:46 by rmhazres         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
TEST_NAME = test_parser

CFLAGS = -Wall -Wextra -Werror -std=c++20
CC = c++
INC = -I./src/common -I./src/http

SRC = src/main.cpp \
      src/common/HttpRequest.cpp \
	  src/common/Utils.cpp \
      src/http/HttpParser.cpp

TEST_SRC = tests/test_parser.cpp \
           src/common/HttpRequest.cpp \
		   src/common/Utils.cpp \
           src/http/HttpParser.cpp

OBJS = $(SRC:.cpp=.o)
TEST_OBJS = $(TEST_SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

test: $(TEST_OBJS)
	$(CC) $(CFLAGS) $(TEST_OBJS) -o $(TEST_NAME)
	./$(TEST_NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -f $(OBJS) $(TEST_OBJS)

fclean: clean
	rm -f $(NAME) $(TEST_NAME)

re: fclean all

.PHONY: all clean fclean re test