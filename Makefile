# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/07 11:50:34 by lyvan-de          #+#    #+#              #
#    Updated: 2026/06/11 16:06:30 by rmhazres         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
TEST_PARSER = test_parser
TEST_VALIDATOR = test_validator
TEST_HTTP_PIPELINE = test_http_pipeline

CFLAGS = -Wall -Wextra -Werror -std=c++20
CC = c++
INC = -I./src/common -I./src/http -I./src/Config -I./tests -I./src/server_setup

SRC =			src/common/HttpRequest.cpp \
	  			src/common/HttpResponse.cpp \
				src/common/Utils.cpp \
				src/http/HttpValidator.cpp \
				src/http/HttpParser.cpp \
				src/http/HttpResponseBuilder.cpp \
				src/http/Router.cpp \
				src/Config/Config.cpp \
				src/Config/ConfigParser.cpp \
				src/Config/Server.cpp \
				src/Config/LocationBlock.cpp

TEST_PARSER_SRC = tests/test_parser.cpp \
                src/common/HttpRequest.cpp \
				src/common/HttpResponse.cpp \
                src/common/Utils.cpp \
                src/http/HttpParser.cpp \
                src/http/HttpValidator.cpp \
				src/Config/Config.cpp \
				src/Config/ConfigParser.cpp \
				src/Config/LocationBlock.cpp \
				src/Config/Server.cpp \
	 			src/server_setup/ASocket.cpp \
	  			src/server_setup/ListeningSocket.cpp

TEST_VALIDATOR_SRC = tests/test_validator.cpp \
                src/common/HttpRequest.cpp \
				src/common/HttpResponse.cpp \
                src/common/Utils.cpp \
                src/http/HttpParser.cpp \
                src/http/HttpValidator.cpp \
				src/Config/Config.cpp \
				src/Config/ConfigParser.cpp \
				src/Config/LocationBlock.cpp \
				src/Config/Server.cpp \
	 			src/server_setup/ASocket.cpp \
	  			src/server_setup/ListeningSocket.cpp \
	  src/Connection/Connection.cpp \
	  src/Connection/EventLoop.cpp \
	  src/Signals/signals.cpp

TEST_HTTP_PIPELINE_SRC = tests/test_http_pipeline.cpp \
                src/common/HttpRequest.cpp \
				src/common/HttpResponse.cpp \
                src/common/Utils.cpp \
				src/http/CGIHandler.cpp \
                src/http/HttpParser.cpp \
				src/http/HttpPipeline.cpp \
				src/http/HttpResponseBuilder.cpp \
                src/http/HttpValidator.cpp \
				src/http/Router.cpp \
				src/Config/Config.cpp \
				src/Config/ConfigParser.cpp \
				src/Config/LocationBlock.cpp \
				src/Config/Server.cpp \
	 			src/server_setup/ASocket.cpp \
	  			src/server_setup/ListeningSocket.cpp

OBJS = $(SRC:.cpp=.o)
TEST_PARSER_OBJS = $(TEST_PARSER_SRC:.cpp=.o)
TEST_VALIDATOR_OBJS = $(TEST_VALIDATOR_SRC:.cpp=.o)
TEST_HTTP_PIPELINE_OBJS = $(TEST_HTTP_PIPELINE_SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(TEST_PARSER): $(TEST_PARSER_OBJS)
	$(CC) $(CFLAGS) $(TEST_PARSER_OBJS) -o $(TEST_PARSER)

$(TEST_VALIDATOR): $(TEST_VALIDATOR_OBJS)
	$(CC) $(CFLAGS) $(TEST_VALIDATOR_OBJS) -o $(TEST_VALIDATOR)
	
$(TEST_HTTP_PIPELINE): $(TEST_HTTP_PIPELINE_OBJS)
	$(CC) $(CFLAGS) $(TEST_HTTP_PIPELINE_OBJS) -o $(TEST_HTTP_PIPELINE)

test:$(TEST_HTTP_PIPELINE) $(TEST_PARSER) $(TEST_VALIDATOR)
	./$(TEST_PARSER)
	./$(TEST_VALIDATOR)
	./$(TEST_HTTP_PIPELINE)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -f $(OBJS) $(TEST_PARSER_OBJS) $(TEST_VALIDATOR_OBJS) $(TEST_HTTP_PIPELINE)

fclean: clean
	rm -f $(NAME) $(TEST_PARSER) $(TEST_VALIDATOR) $(TEST_HTTP_PIPELINE)

re: fclean all

.PHONY: all clean fclean re test