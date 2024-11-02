NAME= ircserv
CMD= c++ -std=c++98 -Wall -Wextra -Werror -MMD -MP #-fsanitize=address
SRC = Core/srcs/Client.cpp Core/srcs/Channel.cpp Core/srcs/Server.cpp Core/srcs/main.cpp \
Core/srcs/IRCMessage.cpp Commands/srcs/AuthNickCmd.cpp Commands/srcs/AuthPassCmd.cpp \
Commands/srcs/MsgPrivmsgCmd.cpp Commands/srcs/MessageCommand.cpp Commands/srcs/ChnlJoinCmd.cpp \
Commands/srcs/ChnlModeCmd.cpp Commands/srcs/ChnlKickCmd.cpp \
Commands/srcs/ChnlInviteCmd.cpp Commands/srcs/ChnlTopicCmd.cpp Commands/srcs/ClientQuitCmd.cpp \
Commands/srcs/ChnlPartCmd.cpp Commands/srcs/AuthUserCmd.cpp Core/srcs/Get.cpp


OBJ = $(SRC:.cpp=.o)
INCLUDES= Core/includes/Client.hpp Core/includes/Channel.hpp Core/includes/Server.hpp \
Core/includes/IRCMessage.hpp Commands/includes/AuthNickCmd.hpp Commands/includes/AuthUserCmd.hpp \
Commnds/includes/AuthPassCmd.hpp Commands/includes/AuthenticationCommand.hpp Commands/includes/MsgPrivmsgCmd.hpp \
Commands/includes/MessageCommand.hpp Commands/srcs/ChnlJoinCmd.hpp \
Commands/includes/ClientQuitCmd.hpp Commands/includes/ChnlPartCmd.hpp Commands/includes/AuthUserCmd.hpp

DEP = $(SRC:.cpp=.d)

$(NAME):	 $(OBJ)
	$(CMD) $(OBJ) -o $(NAME)

%.o:	%.cpp Makefile
	$(CMD) -c $< -o $@

all:		$(NAME)

clean:
	rm -rf $(OBJ) $(DEP)

fclean:		clean
	rm -rf $(NAME)

re:		fclean all

.PHONY: 	all clean fclean re


