NAME = IRC

SRC = \

HEADER_EXTRA = \

HEADERS := $(SRC:.cpp=.hpp)

HEADERS += $(HEADER_EXTRA)

SRC_MAIN = server.cpp

SRC += $(SRC_MAIN)
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -ggdb
CXX = c++
RM = rm

OBJS = $(SRC:.cpp=.o)
DEP = $(OBJS:.o=.d)

all: $(NAME)

-include $(DEP)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c -MMD -MP $< -o $@

$(NAME): $(OBJS) $(HEADERS)
	$(CXX) $(CPPFLAGS) $(OBJS) -o $@

clean:
	$(RM) -rf $(OBJS) $(DEP)

fclean: clean
	$(RM) -f $(NAME)

re: fclean all

run: all
	./$(NAME)

vrun: all
	valgrind ./$(NAME)

.PHONY: clean fclean re all run vrun
