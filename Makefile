NAME = test
CPPC = clang++
CFLAGS = -lcurses
SRC = src/game_logic/*.cpp
OBJ = $(SRC:.cpp=.o)
FRAMEWORKS = -framework OpenGl -w
BREW_INC = -I ~/.brew/include
GLFW_LINK = -L ~/.brew/lib -lglfw

$(NAME):
	@$(CPPC) $(CFLAGS) $(SRC) $(FRAMEWORKS) $(BREW_INC) $(GLFW_LINK) -o $(NAME)

all: $(NAME)

clean:
	@/bin/rm -f $(OBJ)

fclean: clean
	@/bin/rm -f $(NAME)

re:	fclean all
