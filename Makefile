# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/07 11:50:34 by lyvan-de          #+#    #+#              #
#    Updated: 2026/08/12 12:34:00 by lyvan-de         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

CFLAGS = -Wall -Wextra -Werror -std=c++20
CC = c++
INC = -I./src/common -I./src/http -I./src/Config -I./src/ServerSetup

SRC =	src/main.cpp \
		src/Common/HttpRequest.cpp \
	  	src/Common/HttpResponse.cpp \
		src/Common/Utils.cpp \
		src/Config/Config.cpp \
		src/Config/ConfigParser.cpp \
		src/Config/LocationBlock.cpp \
		src/Config/PathUtils.cpp \
		src/Config/Server.cpp \
		src/Connection/Connection.cpp \
		src/Connection/EventLoop.cpp \
		src/Http/CGIHandler.cpp \
		src/Http/CGIProcess.cpp \
		src/Http/HttpParser.cpp \
		src/Http/HttpPipeline.cpp \
		src/Http/HttpResponseBuilder.cpp \
		src/Http/HttpValidator.cpp \
		src/Http/Router.cpp \
		src/ServerSetup/ASocket.cpp \
		src/ServerSetup/ListeningSocket.cpp \
		src/Signals/signals.cpp

OBJS = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -f $(OBJS) $(TEST_PARSER_OBJS) $(TEST_VALIDATOR_OBJS) $(TEST_HTTP_PIPELINE)

fclean: clean
	rm -f $(NAME) $(TEST_PARSER) $(TEST_VALIDATOR) $(TEST_HTTP_PIPELINE)

re: fclean all

.PHONY: all clean fclean re test