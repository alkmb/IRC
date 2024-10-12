NAME= ircserv
CC= g++
CFLAGS= -std=c++11
SRC = Core/src/Client.cpp Core/src/Channel.cpp Core/src/Server.cpp Core/src/main.cpp
OBJ = $(SRC:.cpp=.o)
INCLUDES= Core/includes/Client.hpp Core/includes/Channel.hpp Core/includes/Server.hpp

all: $(NAME)


$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(NAME)

fclean: clean
	rm -rf $(OBJ) .vscode

re: fclean all

.PHONY: all clean fclean re