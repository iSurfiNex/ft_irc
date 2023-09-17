# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/11 18:09:46 by rsterin           #+#    #+#              #
#    Updated: 2023/09/17 18:47:20 by rsterin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

_END = "\033[0m"
_CYAN = "\033[0;36m"
_GREEN = "\033[0;32m"

NAME = ircserv

SRC = src/main.cpp \
	src/parsing/channelJoinLeave.cpp \
	src/parsing/channelManagement.cpp \
	src/parsing/parseInput.cpp \
	src/parsing/parsing.cpp \
	src/parsing/serverLogin.cpp \
	src/parsing/serverMsg.cpp \
	src/utils.cpp \
	src/IrcServer.cpp \
	src/Client.cpp \
	src/Channel.cpp \

HEADERS = includes/IrcServer.hpp \

CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g

ifdef san
   CPPFLAGS += -fsanitize=address,leak,undefined -fno-omit-frame-pointer
endif


CXX = c++
RM = rm


OBJS = $(SRC:.cpp=.o)
DEP = $(OBJS:.o=.d)

all: $(NAME)

-include $(DEP)

%.o: %.cpp
	@echo -n $(_GREEN)
	@echo -n "Compiling: $<..."
	@$(CXX) $(CPPFLAGS) -c -MMD -MP $< -o $@ -I includes/
	@echo -n $(_END)
	@echo

$(NAME): $(OBJS) $(HEADERS)
	@echo -n $(_GREEN)
	@echo -n "Creating executable..."
	@$(CXX) $(CPPFLAGS) $(OBJS) -o $@
	@echo $(_GREEN)
	@echo -n "Done ✓"
	@echo $(_END)
	@echo

clean:
	@echo $(_CYAN)
	@echo "Deleting objs..."
	@$(RM) -rf $(OBJS) $(DEP)
	@echo -n "Done ✓"
	@echo $(_END)
	@echo

fclean: clean
	@echo -n $(_CYAN)
	@echo "Removing executable..."
	@$(RM) -f $(NAME)
	@echo -n "Done ✓"
	@echo $(_END)
	@echo

re: fclean all

run: all
	./$(NAME)

# vrun: all
	valgrind ./$(NAME)

.PHONY: clean fclean re all run vrun
