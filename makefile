NAME = ircserver
SRC = main.cpp Exceptions.cpp Server.cpp
OBJS = $(SRC:.cpp=.o)
INC = Exceptions.hpp Server.hpp 
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp $(INC)
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS)

fclean: clean	
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
