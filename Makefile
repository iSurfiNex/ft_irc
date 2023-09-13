# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/11 18:09:46 by rsterin           #+#    #+#              #
#    Updated: 2023/09/13 15:49:02 by rsterin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

SRC = src/main.cpp \
	src/IrcServer.cpp \
	src/Client.cpp \
	src/Channel.cpp \
	src/parsing/parsing.cpp \
	src/parsing/parseInput.cpp \
	src/parsing/login.cpp \
	src/parsing/join.cpp \
	src/parsing/privMsg.cpp \
	src/utils.cpp \

HEADERS = includes/IrcServer.hpp \


CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -ggdb

CXX = c++
RM = rm


OBJS = $(SRC:.cpp=.o)
DEP = $(OBJS:.o=.d)

all: $(NAME)

-include $(DEP)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c -MMD -MP $< -o $@ -I includes/

$(NAME): $(OBJS) $(HEADERS)
	$(CXX) $(CPPFLAGS) $(OBJS) -o $@

clean:
	$(RM) -rf $(OBJS) $(DEP)

fclean: clean
	$(RM) -f $(NAME)

re: fclean all

run: all
	./$(NAME)

# vrun: all
	valgrind ./$(NAME)

.PHONY: clean fclean re all run vrun
