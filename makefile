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
					Exceptions.cpp \
					Utils.cpp \
					Command.cpp \

ALL_HEADERS			= $(ALL_SRC:.cpp=.hpp)

ALL_LIBS			= empty

PREFIX_SRC			= $(addprefix $(SRC_DIR), $(ALL_SRC))
PREFIX_LIB			= $(addprefix $(LIB_DIR), $(ALL_LIBS))
PREFIX_HEADER		= $(addprefix $(HEAD_DIR), $(ALL_HEADERS))
SRC					= $(wildcard $(PREFIX_SRC))
HEADER				= $(wildcard $(PREFIX_HEADER))
LIB					= $(wildcard $(PREFIX_LIB))
OBJ					= $(patsubst %.cpp, $(OBJ_DIR)%.o, $(notdir $(ALL_SRC)))
DEP					= $(OBJ:.o=.d)

DIRS				= $(OBJ_DIR)

#____________UTILITIES
CC					= c++
CFLAGS				= -Wextra -Wall -Werror -MMD -std=c++98 -g3




#________________________RULES

all : $(NAME)
	$(LOG__ALLSUCCESS)

$(NAME): $(DIRS) $(OBJ)
	$(call logs, $(CYAN),"Compiling\ Executable")
	$(CC) $(CFLAGS) $(OBJ) -I . -o $(NAME)
	$(LOG__SUCCESS)

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
	@mkdir -p $(DIRS)
	$(LOG__SUCCESS)

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
-include $(DEP)

.SILENT:

.PHONY: all clean re fclean force