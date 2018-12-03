NAME = test
CPPC = clang++
CFLAGS = -lcurses
SRC = *.cpp
OBJ = $(SRC:.cpp=.o)

$(NAME):
	@$(CPPC) $(CFLAGS) $(SRC) -o $(NAME)

all: $(NAME)

clean:
	@/bin/rm -f $(OBJ)

fclean: clean
	@/bin/rm -f $(NAME)

re:	fclean all
