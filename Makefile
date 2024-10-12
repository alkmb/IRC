NAME= ircserv
CC= g++
CFLAGS= -std=c++98 -Wall -Wextra -Werror -fsanitize=address
SRC = Core/srcs/Client.cpp Core/srcs/Channel.cpp Core/srcs/Server.cpp Core/srcs/main.cpp \
Core/srcs/IRCMessage.cpp Commands/srcs/AuthNickCmd.cpp Commands/srcs/AuthPassCmd.cpp \
Commands/srcs/MsgPrivmsgCmd.cpp Commands/srcs/MessageCommand.cpp Commands/srcs/ChnlJoinCmd.cpp \
Commands/srcs/ChnlWhoCmd.cpp Commands/srcs/ClientQuitCmd.cpp Commands/srcs/MsgNoticeCmd.cpp

OBJ = $(SRC:.cpp=.o)
INCLUDES= Core/includes/Client.hpp Core/includes/Channel.hpp Core/includes/Server.hpp \
Core/includes/IRCMessage.hpp Commands/includes/AuthNickCmd.hpp Commands/includes/AuthUserCmd.hpp \
Commnds/includes/AuthPassCmd.hpp Commands/includes/AuthenticationCommand.hpp Commands/includes/MsgPrivmsgCmd.hpp \
Commands/includes/MessageCommand.hpp Commands/srcs/ChnlJoinCmd.hpp Commands/srcs/ChnlWhoCmd.hpp \
Commands/includes/ClientQuitCmd.hpp Commands/includes/MsgNoticeCmd.hpp

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
