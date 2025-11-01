#include "ft_ls.h"

// Globals for opti
static int it = 0;
static char print_buff[BUFF_SIZE];
static char buff[BUFF_SIZE];

// ******************** Buffering ********************

inline static void flush()
{
	if (it > 0)
	{
		write(1, print_buff, it);
		it = 0;
	}
}

inline static void fill_buff(char *str, size_t len)
{
	if (it + len >= BUFF_SIZE)
	{
		flush();
		if (len >= BUFF_SIZE)
		{
			write(1, str, len);
			return ;
		}
	}
	ft_memcpy(print_buff + it, str, len);
	it += len;
}

inline static void fill_buff_char(char c)
{
	if (it + 1 >= BUFF_SIZE)
		flush();
	print_buff[it++] = c;
}

// ******************** End Buffering ********************

static void print_list(t_ast *head, t_data *data, t_columns *columns);

// static void print_str_columns(size_t columns_nbr, char *str)
// {
// 	if (columns_nbr)
// 	{
// 		size_t str_len = ft_strlen(str);
// 		size_t space_nbr = columns_nbr - str_len;
// 		char space_buff[space_nbr + 1];
// 		ft_memset(space_buff, 32, space_nbr);
// 		space_buff[space_nbr] = 0;

// 		fill_buff(str, str_len);
// 		fill_buff(space_buff, space_nbr);
// 	}
// 	else
// 		fill_buff(str, ft_strlen(str));
// 	fill_buff_char(' ');
// }

static void put_str_buff(char *str, size_t len)
{
	size_t str_len = ft_strlen(str);
	
	if (len)
	{
		size_t space_nbr = len - str_len;
		ft_memset(buff, 32, space_nbr);
		ft_strlcpy(&buff[space_nbr], str, BUFF_SIZE);
		fill_buff(str, str_len + space_nbr);
	}
	else
		fill_buff(str, str_len);
	fill_buff_char(' ');
}

// static void print_size_t_columns(size_t columns_nbr, size_t nbr)
// {
// 	if (!columns_nbr)
// 	{
// 		size_t tmp = nbr;
// 		while (tmp > 10)
// 		{
// 			columns_nbr++;
// 			tmp /= 10;
// 		}
// 		columns_nbr++;
// 	}
// 	char size_t_buff[22];

// 	ft_memset(size_t_buff, 32, columns_nbr);
// 	size_t_buff[columns_nbr] = 0;
// 	int i = columns_nbr - 1;
// 	while (nbr >= 10)
// 	{
// 		size_t_buff[i--] = (nbr % 10) + 48;
// 		nbr /= 10;
// 	}
// 	if (i >= 0)
// 		size_t_buff[i] = nbr + 48;
// 	fill_buff(size_t_buff, columns_nbr);
// }

static size_t put_size_t_buff(size_t n, size_t len)
{
	if (!len)
	{
		size_t tmp = n;
		while (tmp > 10)
		{
			len++;
			tmp /= 10;
		}
		len++;
	}

	ft_memset(buff, 32, len);
	int i = len - 1;
	while (n >= 10)
	{
		buff[i--] = (n % 10) + 48;
		n /= 10;
	}
	if (i >= 0)
		buff[i] = n + 48;
	return len;
}


// static void print_file_name(t_data *data, t_file *file)
// {
// 	char *str;
// 	char *name = get_name(file);

// 	if (!data->term.is_tty || data->color_parse_error)
// 		fill_buff(name, ft_strlen(name));
// 	else
// 	{
// 		t_map *tmp = get_colors(data->file_colors, data->colors, file);
// 		if (tmp)
// 			str = tmp->value;
// 		else
// 			str = "0";
// 		fill_buff("\e[", 2);
// 		fill_buff(str, ft_strlen(str));
// 		fill_buff_char('m');
// 		fill_buff(name, ft_strlen(name));
// 		fill_buff("\e[0m", 4);
// 	}
// }

static void fill_buff_file_name(t_data *data, t_file *file)
{
	char *str;
	char *name = get_name(file);

	if (!data->term.is_tty || data->color_parse_error)
		fill_buff(name, ft_strlen(name));
	else
	{
		t_map *tmp = get_colors(data->file_colors, data->colors, file);
		if (tmp)
			str = tmp->value;
		else
			str = "0";
		fill_buff("\e[", 2);
		fill_buff(str, ft_strlen(str));
		fill_buff_char('m');
		fill_buff(name, ft_strlen(name));
		fill_buff("\e[0m", 4);
	}
}

static void print_file(t_ast *node, t_data *data, t_columns *columns)
{
	t_file file = node->file_info;
	if (data->flags.l || data->flags.g)
	{
		fill_buff(node->file_info.permissions, 10);
		if (columns->as_acl)
			fill_buff_char(file.acl_char);
		fill_buff_char(' ');
		fill_buff(buff, put_size_t_buff(file.sb.st_nlink, columns->link_max_len));
		fill_buff_char(' ');
		if (!data->flags.g)
			put_str_buff(map_get_id(data->user_id, file.sb.st_uid)->value, columns->user_max_len);
		put_str_buff(map_get_id(data->group_id, file.sb.st_gid)->value, columns->group_max_len);
		if (file.type == TYPE_BLOCK || file.type == TYPE_CHR)
		{	
			fill_buff(buff, put_size_t_buff(major(file.sb.st_rdev), columns->major_max_len));
			fill_buff(", ", 2);
			fill_buff(buff, put_size_t_buff(minor(file.sb.st_rdev), columns->minor_max_len));
		}
		else
			fill_buff(buff, put_size_t_buff(file.sb.st_size, columns->size_max_len));
		fill_buff_char(' ');
		if (data->flags.u)
			put_size_t_buff(file.sb.st_atime, 0);
		else
			put_size_t_buff(file.sb.st_mtime, 0);
		fill_buff_char(' ');
		fill_buff_file_name(data, &file);
		if (file.type == TYPE_LINK)
		{
			fill_buff(" -> ", 4);
			fill_buff_file_name(data, file.redirect_file);
		}
	}
	else
		fill_buff_file_name(data, &file);
}

// static void print_file(t_ast *node, t_data *data, t_columns *columns)
// {
// 	if (data->flags.l || data->flags.g)
// 	{
// 		t_file file = node->file_info;

// 		fill_buff(file.permissions, ft_strlen(file.permissions));
// 		if (columns->as_acl)
// 			fill_buff_char(file.acl_char);
// 		fill_buff_char(' ');
// 		print_size_t_columns(columns->link_max_len, file.link);
// 		fill_buff_char(' ');
// 		if (!data->flags.g)
// 			print_str_columns(columns->user_max_len, file.user_name);
// 		print_str_columns(columns->group_max_len, file.group_name);
// 		if (file.type == TYPE_BLOCK || file.type == TYPE_CHR)
// 		{
// 			print_size_t_columns(columns->major_max_len, file.major);
// 			fill_buff(", ", 2);
// 			print_size_t_columns(columns->minor_max_len, file.minor);
// 		}
// 		else
// 			print_size_t_columns(columns->size_max_len, file.size);
// 		fill_buff_char(' ');
// 		if (data->flags.u)
// 			fill_buff(file.access_time, ft_strlen(file.access_time));
// 		else
// 			fill_buff(file.mod_time, ft_strlen(file.mod_time));
// 		fill_buff_char(' ');
// 		print_file_name(data, &file);
// 		if (file.type == TYPE_LINK)
// 		{
// 			fill_buff(" -> ", 4);
// 			print_file_name(data, file.redirect_file);
// 		}
// 	}
// 	else
// 		print_file_name(data, &node->file_info);
// }

static size_t get_total_blocks(t_ast *node)
{
	size_t blocks = 0;
	t_ast *tmp = node->head;
	while (tmp)
	{
		blocks += tmp->file_info.sb.st_blocks;
		tmp = tmp->next;
	}
	return (blocks / 2);
}

static void print_folder(t_ast *node, t_data *data, int print_path)
{
	if ((print_path && data->flags.R && !data->flags.d) || (print_path == 2 && !data->flags.d))
	{
		fill_buff(node->path, ft_strlen(node->path));
		fill_buff(":\n", 2);
	}
	if (node->file_info.error == OPENDIR_ERROR)
	{
		flush();
		opendir_error(node->path);
	}
	else if (data->flags.l && !data->flags.d)
	{
		if (!node->file_info.error)
		{
			fill_buff("total ", 6);
			flush();
			put_size_t_buff(get_total_blocks(node), 0);
			fill_buff_char('\n');
		}
	}
	t_columns *columns = parse_columns(data, node);
	if (!columns)
		free_all_and_exit(&data, 2);
	if (data->flags.d)
		print_file(node, data, columns);
	else
		print_list(node->head, data, columns);
	free(columns);
}

// static void print_folder(t_ast *node, t_data *data, int print_path)
// {
// 	if ((print_path && data->flags.R && !data->flags.d) || (print_path == 2 && !data->flags.d))
// 	{
// 		fill_buff(node->path, ft_strlen(node->path));
// 		fill_buff(":\n", 2);
// 	}
// 	if (node->file_info.error == OPENDIR_ERROR)
// 	{
// 		flush();
// 		opendir_error(node->path);
// 	}
// 	else if (data->flags.l && !data->flags.d)
// 	{
// 		size_t blocks = 0;
// 		t_ast *tmp = node->head;
// 		while (tmp)
// 		{
// 			blocks += tmp->file_info.block_size;
// 			tmp = tmp->next;
// 		}
// 		if (!node->file_info.error)
// 		{
// 			fill_buff("total ", 6);
// 			flush();
// 			ft_printf("%d", blocks / 2);
// 			fill_buff_char('\n');
// 		}
// 	}
// 	t_columns *columns = parse_columns(node);
// 	if (!columns)
// 		free_all_and_exit(&data, 2);
// 	if (data->flags.d)
// 		print_file(node, data, columns);
// 	else
// 		print_list(node->head, data, columns);
// 	free(columns);
// }

static void print_list(t_ast *head, t_data *data, t_columns *columns)
{
	if (!head)
		return ;
	t_ast	**array = convert_to_array(head);
	if (!array)
		free_all_and_exit(&data, 2);
	sort_array(&array, data->flags);
	for (int i = 0; array[i]; i++)
	{
		print_file(array[i], data, columns);
		if (array[i + 1])
		{
			if (data->flags.l || data->flags.g || !data->term.is_tty)
				fill_buff_char('\n');
			else
				fill_buff("  ", 2);
		}
	}
	fill_buff_char('\n');
	if (data->flags.R)
	{
		for (int i = 0; array[i]; i++)
		{
			if (array[i]->file_info.type == TYPE_DIR)
			{
				if (strncmp("..", get_name(&array[i]->file_info), 3) && strncmp(".", get_name(&array[i]->file_info), 2))
				{
					fill_buff_char('\n');
					print_folder(array[i], data, 1);
				}	
			}
		}
	}
	free(array);
}

static void print_node(t_data *data, t_ast *node, t_ast *next_node, int index)
{
	t_columns *columns;

	if (node->file_info.type == TYPE_DIR)
	{
		if (!index && !next_node && !data->flags.R)
			print_folder(node, data, 0);
		else
			print_folder(node, data, 2);
		if (next_node && !data->flags.d)
			fill_buff_char('\n');
		else if (next_node)
			fill_buff("  ", 2);
	}
	else
	{
		if (data->flags.l || data->flags.g)
		{
			columns = parse_columns(data, node);
			if (!columns)
				free_all_and_exit(&data, 2);
			print_file(node, data, columns);
			fill_buff_char('\n');
			free(columns);
		}
		else
			print_file(node, data, NULL);
		if (data->flags.l)
			return ;
		if (next_node && next_node->file_info.type != TYPE_DIR)
			fill_buff("  ", 2);
		else
			fill_buff("\n", 1);
	}
}

void print(t_data *data)
{	
	t_ast	**array = convert_to_array(data->tree);
	if (!array)
		free_all_and_exit(&data, 2);

	sort_array(&array, data->flags);

	if (data->term.is_tty && data->color_parse_error)
		ft_putstr_fd("ft_ls: unparsable value for LS_COLORS environment variable\n", 2);
	for (int i = 0; array[i]; i++)
	{
		switch (array[i]->file_info.error)
		{
		case STAT_ERROR:
			flush();
			stat_error(array[i]->path);
			break;
		case OPENDIR_ERROR:
			flush();
			opendir_error(array[i]->path);
			break;
		default:
			print_node(data, array[i], array[i + 1], i);
			break;
		}
	}
	if (data->flags.d)
		fill_buff_char('\n');
	free(array);
	flush();
}
