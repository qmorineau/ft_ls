/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qmorinea <qmorinea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 10:59:34 by qmorinea          #+#    #+#             */
/*   Updated: 2025/11/01 03:40:29 by qmorinea         ###   ########.fr       */
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

// Import
# include "libft.h"

# define TYPE_FILE 0
# define TYPE_DIR 1
# define TYPE_LINK 2
# define TYPE_BLOCK 3
# define TYPE_PIPE 4
# define TYPE_SOCKET 5
# define TYPE_CHR 6 //character device => /dev/null
# define TYPE_UNKNOWN 7
# define TYPE_BROKEN_LINK 8

# define MONTH_IN_SEC 2629746

#define BUFF_SIZE 16384

// Structures
typedef struct s_map
{
	char			*key;
	char			*value;
	struct s_map	*next;
}	t_map;

typedef struct s_map_uid
{
	uid_t				key;
	char				*value;
	struct s_map_uid	*next;
}	t_map_uid;

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
	int				type;
	enum e_name_type {
		BUFFER = 0,
		PTR = 1
	} name_type;
	union u_name {
		char	buff[257];
		char 	*ptr;
	}	name;

	enum e_error {
		NO_ERROR = 0,
		STAT_ERROR = 1,
		OPENDIR_ERROR = 2
	} error;

	size_t time;
	struct s_file	*redirect_file;
	struct stat		sb;
	// struct passwd	*pw;
	// struct group	*gr;
	char			acl_char;
	char			permissions[11];

	// unsigned int	link;
	// unsigned int	major;
	// unsigned int	minor;
	// char			*user_name;
	// char			*group_name;
	// char			mod_time[13];
	// char			access_time[13];
	// size_t			raw_mod_time;
	// size_t			raw_access_time;
	// size_t			size;
	// size_t			block_size;
}	t_file;

typedef struct s_ast
{
	char			*path;
	t_file			file_info;
	struct s_ast	*next;
	struct s_ast	*tail; // tail is only on the head of the list
	struct s_ast	*head;
	size_t			index;
}	t_ast;

typedef struct s_data
{
	t_flags		flags;
	t_ast		*tree;
	t_map		*colors;
	t_map		*file_colors;
	t_map_uid	*user_id;
	t_map_uid	*group_id;
	t_terminfo	term;
	size_t		now;
	int			color_parse_error;
	int			exit_status;
}	t_data;

typedef struct s_columns
{
	size_t	user_max_len;
	size_t	group_max_len;
	size_t	size_max_len;
	size_t	minor_max_len;
	size_t	major_max_len;
	size_t	link_max_len;
	int		as_acl;
}	t_columns;

// Node Functions
t_ast			*new_ast_node();
void			ast_addback(t_ast **head, t_ast *new);
unsigned int	ast_length(t_ast *head);
void			ast_clear(t_ast **node);

// Parsing
int				option_parser(int argc, char* argv[], t_flags *flags);
int				parse_arguments(int argc, char *argv[], t_data *data);
int				parse_data(t_data *data);
int				parse_ast_node(t_data *data, t_ast **parent);
void			parse_file_infos(t_data *data, t_ast **node);
void			parse_colors(t_data *data, char *envp[]);
t_columns	*parse_columns(t_data *data, t_ast *node);
int 			dirent_type_parser(struct dirent *entry);
int				stat_type_parser(struct stat *buff);

int match_file_patern(char **ext);
ssize_t get_index(char *str, char c);

// Colors
t_map *get_colors(t_map *file_colors, t_map *colors, t_file *file);

// Stat
	void parse_permissions(struct stat *buff, t_file *file);
void parse_file_from_stat(t_data *data, t_file *file);
int parse_modified_time(t_data *data, t_file *file, struct stat *buff);
int parse_access_time(t_data *data, t_file *file, struct stat *buff);
t_file *parse_link(struct stat *sb, char *path);
// Terminal
void			parse_terminal(t_terminfo *term_struct);

// Convert
t_ast			**convert_to_array(t_ast *head);

// Sort
void			sort_array(t_ast ***array, t_flags flags);

// Print
void			print(t_data *data);

// Utils
void			free_all_and_exit(t_data **data, int exit_code);
void			free_file_info(t_file *file);
char *get_name(t_file *file);

// Map
int				map_set(t_map **map, char **key, char **value);
t_map			*map_get(t_map *map, char *key);
t_map			*find_extension(t_map *map, char *name);
t_map_uid *map_get_id(t_map_uid *map, uid_t id);
int map_set_uid(t_map_uid **map, uid_t id, char **value);

// Access Control List
char			get_acl(char *path);

// Errors
void stat_error(char *path);
void opendir_error(char *path);

#endif