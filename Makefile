NAME= ircServer
CC-= g++
CFLAGS= -Wall -Wextra -Werror -std=c++98
SRC= src/main.cpp src/ircServer.cpp src/Client.cpp
INCLUDES = includes/ircServer.hpp includes/Client.hpp
OBJ= $(SRC:.cpp=.o)
LDFLAGS= -lstdc++

all: $(NAME)

$(NAME): $(OBJ) $(INCLUDES)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

