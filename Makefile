# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -I $(LIBFT_INC) -fsanitize=address -g

# Directories
SRC_DIR = srcs
OBJ_DIR = .obj
OBJ_FOLDER = obj
LIBFT_DIR = libft

# Name
NAME = ft_ls
LIBFT = $(LIBFT_DIR)/libft.a

# Header
INC = includes
LIBFT_INC = libft/includes

# Source Directories
DIR = parser\
		sorter\

# Source and Object files
SRC_LIST = main.c\
			utils.c\
			parser/data_parser.c\
			parser/option_parser.c\

SRC = $(addprefix $(SRC_DIR)/,$(SRC_LIST))
OBJ = $(addprefix $(OBJ_DIR)/,$(SRC_LIST:.c=.o))

# Colors
YELLOW = \033[0;33m
RED = \033[0;31m
RESET = \033[0m

# Main
all: $(OBJ_DIR) $(NAME)

# Linking object files
$(NAME): $(OBJ_DIR) $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) -I $(INC) $(OBJ) $(LIBFT) -o $(NAME)
	@echo "$(YELLOW)Exec $(NAME) created.$(RESET)"

# Compiling source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC) $(LIBFT_INC)
	@$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(addprefix $(OBJ_DIR)/,$(DIR))

$(LIBFT):
	@make -C $(LIBFT_DIR) --no-print-directory

clean:
	@rm -rf $(OBJ_FOLDER)
	@make clean -C $(LIBFT_DIR) --no-print-directory
	@echo "$(RED)$(NAME): Cleaned object files$(RESET)"

fclean:
	@rm -f $(NAME)
	@rm -rf $(OBJ_FOLDER)
	@make fclean -C $(LIBFT_DIR) --no-print-directory
	@echo "$(RED)$(NAME): Removed binary files$(RESET)"

re: fclean all

norm:
	@norminette includes
	@norminette libft
	@norminette src

test: all
# 	./$(NAME)
# 	./$(NAME) libft
# 	./$(NAME) "-la"

test_flag: all
	./$(NAME) -la;
	./$(NAME) -R;
	./$(NAME) -a;
	./$(NAME) -r;
	./$(NAME) -t;
	./$(NAME) -u;
	./$(NAME) -f;
	./$(NAME) -g;
	./$(NAME) -d;

.PHONY: all clean fclean re norm test