# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -I $(LIBFT_INC) -O2 -march=native -flto -fomit-frame-pointer -g #-fsanitize=address

# Directories
SRC_DIR = srcs
OBJ_DIR = .obj
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
		printer\
		converter\
		utils\

# Source and Object files
SRC_LIST = main.c\
			parser/args_parser.c\
			parser/colors_parser.c\
			parser/data_parser.c\
			parser/extra_parser.c\
			parser/file_patern_parser.c\
			parser/option_parser.c\
			parser/parse_columns.c\
			parser/permissions_parser.c\
			parser/stat_parser.c\
			parser/term_parser.c\
			parser/type_parser.c\
			printer/print.c\
			printer/buffer_error_writer.c\
			printer/errors.c\
			converter/linklist_to_array.c\
			sorter/quicksort.c\
			utils/utils.c\
			utils/free.c\
			utils/map.c\
			utils/ast.c\
			utils/path.c\

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
	@$(CC) $(CFLAGS) -I $(INC) $(OBJ) $(LIBFT) -o $(NAME) -lacl
	@echo "$(YELLOW)Exec $(NAME) created.$(RESET)"

# Compiling source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC) $(LIBFT_INC)
	@$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(addprefix $(OBJ_DIR)/,$(DIR))

$(LIBFT):
	@make -C $(LIBFT_DIR) --no-print-directory

clean:
	@rm -rf $(OBJ_DIR)
	@make clean -C $(LIBFT_DIR) --no-print-directory
	@echo "$(RED)$(NAME): Cleaned object files$(RESET)"

fclean:
	@rm -f $(NAME)
	@rm -rf $(OBJ_DIR)
	@make fclean -C $(LIBFT_DIR) --no-print-directory
	@echo "$(RED)$(NAME): Removed binary files$(RESET)"

re: fclean all

test: all
	-bash tester.sh

colors: all
	LS_COLORS='rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00:su=37;41:sg=30;43:ca=00:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.avif=01;35:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.webp=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.m4a=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.oga=00;36:*.opus=00;36:*.spx=00;36:*.xspf=00;36:*~=00;90:*#=00;90:*.bak=00;90:*.crdownload=00;90:*.dpkg-dist=00;90:*.dpkg-new=00;90:*.dpkg-old=00;90:*.dpkg-tmp=00;90:*.old=00;90:*.orig=00;90:*.part=00;90:*.rej=00;90:*.rpmnew=00;90:*.rpmorig=00;90:*.rpmsave=00;90:*.swp=00;90:*.tmp=00;90:*.ucf-dist=00;90:*.ucf-new=00;90:*.ucf-old=00;90:' ./$(NAME)

.PHONY: all clean fclean re test