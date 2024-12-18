#________________________MACROS

NAME				= ircserv

#____________DEBUGGING
RESET				= \033[0m
UNDERLINE			= \033[4m
BOLD				= \033[1m
RED					= \033[0;91m
GREEN				= \033[0;92m
YELLOW				= \033[0;93m
BLUE				= \033[0;94m
PURPLE				= \033[0;35m
CYAN				= \033[0;96m
WHITE				= \033[0;37m

define	logs
	@printf "$(1)\r\t $(2) $(RESET)"
endef

LOG__PREL			= @echo "$(PURPLE) \nMAKEFILE $(RESET)\n"
LOG__SUCCESS		= @printf "$(GREEN)\rDONE $(RESET)\n"
LOG__ALLSUCCESS		= @printf "\033[1;92m\n\rALL DONE $(RESET)\n\n"

#____________DIRECTORIES
SRC_DIR				= src/
HEAD_DIR			= src/
LIB_DIR				= lib/
OBJ_DIR				= $(SRC_DIR).build/

#____________FILES
ALL_SRC				= main.cpp \
					Server.cpp \
					Client.cpp \
					Channel.cpp \
					Exceptions.cpp \
					Utils.cpp \
					NumericReplies.cpp \
					commands/join.cpp \
					commands/nick.cpp \
					commands/user.cpp \
					commands/privmsg.cpp \
					commands/who.cpp \
					commands/topic.cpp \
					commands/pass.cpp \
					commands/mode.cpp \
					commands/invite.cpp \
					commands/kick.cpp \
					commands/part.cpp \
					commands/pong.cpp \
					commands/motd.cpp \
					commands/quit.cpp \
					commands/list.cpp \

ALL_HEADERS			= $(ALL_SRC:.cpp=.hpp)

ALL_LIBS			= empty

PREFIX_SRC			= $(addprefix $(SRC_DIR), $(ALL_SRC))
PREFIX_LIB			= $(addprefix $(LIB_DIR), $(ALL_LIBS))
PREFIX_HEADER		= $(addprefix $(HEAD_DIR), $(ALL_HEADERS))
# SRC					= $(wildcard $(PREFIX_SRC))
# HEADER				= $(wildcard $(PREFIX_HEADER))
# LIB					= $(wildcard $(PREFIX_LIB))

# Preserve subdirectory structure for object files
OBJ					= $(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(PREFIX_SRC))
DEP					= $(OBJ:.o=.d)

DIRS				= $(sort $(dir $(OBJ)))

#____________UTILITIES
CC					= c++
CFLAGS				= -Wextra -Wall -Werror -MMD -std=c++98 -g3

#________________________RULES

all : $(NAME)
	$(LOG__ALLSUCCESS)

-include $(DEP)

$(NAME): $(DIRS) $(OBJ)
	$(call logs, $(CYAN),"Compiling\ Executable")
	$(CC) $(CFLAGS) $(OBJ) -I . -o $(NAME)
	$(LOG__SUCCESS)

# Compile .o files, maintaining folder structure
$(OBJ_DIR)%.o : $(SRC_DIR)%.cpp
	$(call logs, $(CYAN),"Compiling\ OBJ\ files")
	$(CC) $(CFLAGS) -c $< -o $@
	$(LOG__SUCCESS)

$(LIB) : force
	$(call logs, $(CYAN),"Compiling\ LIBS")
	@make -sC $(LIB_DIR)
	$(LOG__SUCCESS)

$(DIRS) :
	$(call logs, $(CYAN),"Creating\ directories")
	@mkdir -p $@
	$(LOG__SUCCESS)

bot : src/bot/Omegatron_9000.cpp
	$(CC) -Wextra -Wall -Werror -std=c++98 -g3 src/bot/Omegatron_9000.cpp -o Omegatron_9000

clean : 
	$(call logs, $(YELLOW),"Cleaning\ OBJ\ files")
	rm -rf $(OBJ_DIR)
	$(LOG__SUCCESS)

fclean : clean
	$(call logs, $(YELLOW),"Cleaning\ Executable")
	rm -f $(NAME)
	$(LOG__SUCCESS)

re : fclean all

#____________RESSOURCES
