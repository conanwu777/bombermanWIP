NAME = test
CPPC = clang++
CFLAGS = -lcurses
SRC = srcs/*.cpp engine/srcs/*.cpp 
OBJ = $(SRC:.cpp=.o)
FRAMEWORKS = -framework OpenGl -w
BREW_INC = -I ~/.brew/include
GLFW_LINK = -L ~/.brew/lib -lglfw

RED = "\033[1;38;2;225;20;20m"
ORANGE = "\033[1;38;2;255;120;10m"
YELLO = "\033[1;38;2;255;200;0m"
GREEN = "\033[1;38;2;0;170;101m"
LG = "\033[1;38;2;167;244;66m"
BLUE = "\033[1;38;2;50;150;250m"
PURPLE = "\033[1;38;2;150;75;255m"
WHITE = "\033[1;38;2;255;250;232m"

$(NAME): $(SRC)
	@echo $(WHITE)Making $(NAME)
	@$(CPPC) $(CFLAGS) $(SRC) $(FRAMEWORKS) $(BREW_INC) $(GLFW_LINK) -o $(NAME)
	@echo $(GREEN)"Done! =)"

all: $(NAME)

clean:
	@echo $(RED)Cleaning o files
	@/bin/rm -f *.o

fclean: clean
	@echo $(RED)Removing excutable
	@/bin/rm -f $(NAME)

re:	fclean all