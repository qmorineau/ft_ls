#ifndef FT_LS
#define FT_LS

// Includes
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <sys/xattr.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// Import
#include "libft.h"

// Macros
# define TYPE_FILE 0
# define TYPE_FOLDER 1

// Structures
typedef struct s_flags {
	int	l;
	int R;
	int a;
	int r;
	int t;
	// Bonus
	int u;
	int f;
	int g;
	int d;
} t_flags;

typedef struct s_file {
	char *name;
} t_file;

typedef struct s_ast {
	char *path;
	int type;
	t_file file_info;
	struct s_ast *next;
	struct s_ast *head;
} t_ast;

typedef struct s_data {
	t_flags flags;
	t_ast **args;
} t_data;

// Node Functions
t_ast *new_ast_node(int type);
void ast_addfront(t_ast **head, t_ast *new);
unsigned int ast_length(t_ast *head);
void ast_clear(t_ast **node);

// Parsing
int	option_parser(int argc, char* argv[], t_flags *flags);
int	parse_arguments(int argc, char *argv[], t_data *data);
int parse_data(t_data *data);

void free_all(t_data **data);
// to remove ?
void error(char *error);

#endif