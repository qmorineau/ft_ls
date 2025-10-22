/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qmorinea <qmorinea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 10:59:34 by qmorinea          #+#    #+#             */
/*   Updated: 2025/10/23 00:49:54 by qmorinea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

// Includes
# include <unistd.h>
# include <sys/types.h>
# include <dirent.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <pwd.h>
# include <grp.h>
# include <sys/xattr.h>
# include <time.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <grp.h>
// Import
# include "libft.h"

// Macros
# define TYPE_FILE 0
# define TYPE_FOLDER 1

// Structures
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
	char			*name;
	char			permissions[4];
	unsigned int	link;
	char			*user_name;
	char			*group_name;
	char			*time;
	char			*access_time;
	size_t			raw_time;
	size_t			size;
	size_t			block_size;
}	t_file;

typedef struct s_ast
{
	char			*path;
	t_file			file_info;
	struct s_ast	*next;
	// tail is only on the head of the list
	struct s_ast	*tail;
	struct s_ast	*head;
}	t_ast;

typedef struct s_data
{
	t_flags	flags;
	t_ast	*tree;
}	t_data;

typedef struct s_columns
{
	size_t	user_max_len;
	size_t	group_max_len;
	size_t	size_max_len;
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
int				parse_ast_node(t_ast **parent, t_flags flags);
int				parse_file_infos(t_ast **node, t_flags flags);
t_columns		*parse_columns(t_ast *node);

// Stat
int parse_file_type(struct stat *buff);
void parse_permissions(struct stat *buff, char str_buff[1][4]);
char *parse_group(struct stat *buff);
char *parse_user(struct stat *buff);
char *parse_time(struct stat *buff);
char *parse_access_time(struct stat *buff);

// Convert
t_ast			**convert_to_array(t_ast *head);

// Sort
void sort_array(t_ast ***array, t_flags flags);

// Print
void			print(t_data *data);

// Utils
void			free_all(t_data **data);

///////////////////////////// TEST
// to remove ?
int test_ascii(t_ast *node1, t_ast *node2);
void			error(char *error);
void get_attributes(char *path);


#endif