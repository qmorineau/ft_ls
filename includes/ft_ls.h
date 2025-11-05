/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qmorinea <qmorinea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 10:59:34 by qmorinea          #+#    #+#             */
/*   Updated: 2025/11/05 18:08:26 by qmorinea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

// Includes
# include <unistd.h>
# include <sys/types.h>
# include <dirent.h>
# include <sys/stat.h>
# include <pwd.h>
# include <grp.h>
# include <sys/xattr.h>
# include <time.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <grp.h>
# include <termio.h>
# include <sys/acl.h>
# include <sys/sysmacros.h>
# include <errno.h>
# include <linux/limits.h>

// Import
# include "libft.h"

# define MONTH_IN_SEC 2629746
# define BUFF_SIZE 16384
# define POOL_ITEMS_NUMBER 32

typedef enum e_map_type
{
	STR = 0,
	UID = 1
}	t_map_type;

typedef struct s_map
{
	t_map_type	type;
	union u_key {
		char *str;
		uid_t uid;
	}	key;
	char *value;
	size_t	len;
}	t_map;

typedef struct s_map_pool
{
	t_map_type			type;
	t_map				pool[POOL_ITEMS_NUMBER];
	int					it;
	struct s_map_pool	*next;
}	t_map_pool;

typedef struct s_terminfo
{
	int				is_tty;
	unsigned int	width;
}	t_terminfo;

typedef struct s_flags
{
	int	l;
	int	R;
	int	a;
	int	r;
	int	t;
	// Bonus
	int	u;
	int	f;
	int	g;
	int	d;
}	t_flags;

typedef struct s_file
{
	enum e_file_type {
		TYPE_FILE = 0,
		TYPE_DIR = 1,
		TYPE_LINK = 2,
		TYPE_BLOCK = 3,
		TYPE_PIPE = 4,
		TYPE_SOCKET = 5,
		TYPE_CHR = 6, //character device => /dev/null
		TYPE_UNKNOWN = 7,
		TYPE_BROKEN_LINK = 8
	} type;
	enum e_name_type {
		E_FILE = 0,
		E_PATH = 1,
		E_PTR = 2
	} name_type;
	union u_name {
		char	buff[257];
		char 	path[PATH_MAX];
		char	*ptr;
	}	name;

	int error;
	time_t			time;
	char			time_buff[13];
	struct s_file	*redirect_file;
	struct stat		sb;
	char			acl_char;
	char			permissions[11];
}	t_file;

typedef struct s_ast
{
	t_file			file_info;
	size_t			index;
}	t_ast;

typedef struct s_pool_ast
{
	t_ast	pool[POOL_ITEMS_NUMBER];
	int		it;
	struct s_pool_ast *next;
}	t_pool_ast;

typedef struct s_data
{
	int			color_parse_error;
	int			exit_status;
	int			first_print;
	char		path[PATH_MAX];
	size_t		path_len;
	size_t		now;
	t_map_pool	*colors;
	t_map_pool	*file_colors;
	t_map_pool	*user_id;
	t_map_pool	*group_id;
	t_terminfo	term;
	t_flags		flags;
}	t_data;

typedef struct s_columns
{
	size_t	user;
	size_t	group;
	size_t	size;
	size_t	minor;
	size_t	major;
	size_t	link;
	short	acl;
}	t_columns;

// Parser
int				option_parser(int argc, char* argv[], t_flags *flags);
void			parse_folder(t_data *data, t_file *file, int is_header);
int				parse_file_infos(t_data *data, t_ast **node);
int 			dirent_type_parser(struct dirent *entry);
int				stat_type_parser(struct stat *buff);
void			parse_columns(t_columns *columns, t_data *data, t_ast **array);

// Converter
t_ast			**convert_to_array(t_pool_ast *pool);

// Sorter
void			sort_array_args(t_ast ***array, t_flags flags);
void			sort_array(t_ast ***array, t_flags flags);

// Printer
void			print(t_data *data, t_ast *head);
void			print_file(t_file file, t_data *data, t_columns *columns);
void			print_header(t_data *data, t_ast **array, t_file *file, int print_path);
void			print_folder_files_list(t_data *data, t_ast **array, t_columns *columns);

// Colors
t_map			*get_colors(t_map_pool *file_colors, t_map_pool *colors, t_file *file);
void			parse_colors(t_data *data, char *envp[]);
int				match_file_patern(char **ext);
ssize_t			get_index(char *str, char c);

// Stat
void			parse_file_from_stat(t_data *data, t_file *file);
void			parse_permissions(struct stat *buff, t_file *file);
t_file			*parse_link(t_data *data);

// Terminal
void			parse_terminal(t_terminfo *term_struct);

// Utils
char			*get_name(t_file *file);
void			free_all_and_exit(t_data *data, int exit_code);

// Map
t_map			*map_get(t_map_pool *map, void *key);
t_map			*find_extension(t_map_pool *map, char *name);
int				map_set(t_map_pool **map, void *key, char *value, t_map_type type);
void			map_pool_clear(t_map_pool **pool_head);

// AST
t_ast*			get_new_ast(t_pool_ast **head);
void			ast_pool_clear(t_pool_ast **pool_head);

// Access Control List
char			get_acl(char *path);

// Errors
void stat_error(char *path);
void opendir_error(char *path);

// Path
void pop_path(t_data *data);
void push_path(t_data *data, t_file *folder);

// to delete
void test_flush();

#endif
