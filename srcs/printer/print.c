#include "ft_ls.h"

#define BUFF_SIZE 8192

static int it = 0;
static char buff[BUFF_SIZE];

inline static void flush()
{
	if (it > 0)
	{
		write(1, buff, it);
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
	ft_memcpy(buff + it, str, len);
	it += len;
}

inline static void fill_buff_char(char c)
{
	if (it + 1 >= BUFF_SIZE)
		flush();
	buff[it++] = c;
}

static void print_list(t_ast *head, t_data *data, t_columns *columns);

static void print_str_columns(size_t columns_nbr, char *str)
{
	if (columns_nbr)
	{
		size_t str_len = ft_strlen(str);
		size_t space_nbr = columns_nbr - str_len;
		char space_buff[space_nbr + 1];
		ft_memset(space_buff, 32, space_nbr);
		space_buff[space_nbr] = 0;

		fill_buff(str, str_len);
		fill_buff(space_buff, space_nbr);
	}
	else
		fill_buff(str, ft_strlen(str));
	fill_buff_char(' ');
}

static void print_size_t_columns(size_t columns_nbr, size_t nbr)
{
	char *str = ft_calloc(columns_nbr + 1, sizeof(char));
	if (!str)
		exit(2); // manage error
	ft_memset(str, 32, columns_nbr);
	int i = columns_nbr - 1;
	while (nbr >= 10)
	{
		str[i--] = (nbr % 10) + 48;
		nbr /= 10;
	}
	if (i >= 0)
		str[i] = nbr + 48;
	fill_buff(str, ft_strlen(str));
	free(str);
}

static void print_file_name(t_data *data, t_file *file)
{
	char *str;
	if (!data->term.is_tty || data->color_parse_error)
		fill_buff(file->name, ft_strlen(file->name));
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
		fill_buff(file->name, ft_strlen(file->name));
		fill_buff("\e[0m", 4);
	}
}

static void print_file(t_ast *node, t_data *data, t_columns *columns)
{
	if (data->flags.l || data->flags.g)
	{
		t_file file = node->file_info;

		fill_buff(file.permissions, ft_strlen(file.permissions));
		if (columns->as_acl)
			fill_buff_char(file.acl_char);
		fill_buff_char(' ');
		print_size_t_columns(columns->link_max_len, file.link);
		fill_buff_char(' ');
		if (!data->flags.g)
			print_str_columns(columns->user_max_len, file.user_name);
		print_str_columns(columns->group_max_len, file.group_name);
		if (file.type == TYPE_BLOCK || file.type == TYPE_CHR)
		{
			print_size_t_columns(columns->major_max_len, file.major);
			fill_buff(", ", 2);
			print_size_t_columns(columns->minor_max_len, file.minor);
		}
		else
			print_size_t_columns(columns->size_max_len, file.size);
		fill_buff_char(' ');
		if (data->flags.u)
			fill_buff(file.access_time, ft_strlen(file.access_time));
		else
			fill_buff(file.mod_time, ft_strlen(file.mod_time));
		fill_buff_char(' ');
		print_file_name(data, &file);
		if (file.type == TYPE_LINK)
		{
			fill_buff(" -> ", 4);
			print_file_name(data, file.redirect_file);
		}
	}
	else
		print_file_name(data, &node->file_info);
}

static void print_folder(t_ast *node, t_data *data, int print_path)
{
	if ((print_path && data->flags.R && !data->flags.d) || print_path == 2)
	{
		fill_buff(node->path, ft_strlen(node->path));
		fill_buff(":\n", 2);
	}
	if (node->file_info.stat_error)
	{
		flush();
		ft_putstr_fd("ft_ls: cannot open directory '", 2);
		ft_putstr_fd(node->path, 2);
		struct stat sb;
		stat(node->path, &sb);
		perror("'");
	}
	else if (data->flags.l && !data->flags.d)
	{
		size_t blocks = 0;
		t_ast *tmp = node->head;
		while (tmp)
		{
			blocks += tmp->file_info.block_size;
			tmp = tmp->next;
		}
		if (!node->file_info.stat_error)
		{
			fill_buff("total ", 6);
			flush();
			ft_printf("%d", blocks / 2);
			fill_buff_char('\n');
		}
	}
	t_columns *columns = parse_columns(node);
	if (!columns)
		exit(2); // manage error
	if (data->flags.d)
	{
		print_file(node, data, columns);
		fill_buff_char('\n');
	}
	else
		print_list(node->head, data, columns);
	free(columns);
}

static void print_list(t_ast *head, t_data *data, t_columns *columns)
{
	if (!head)
		return ;
	t_ast	**array = convert_to_array(head);
	if (!array)
		exit(2); // manage error
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
				if (strncmp("..", array[i]->file_info.name, 3) && strncmp(".", array[i]->file_info.name, 2))
				{
					fill_buff_char('\n');
					print_folder(array[i], data, 1);
				}	
			}
		}
	}
	free(array);
}

void print(t_data *data)
{	
	t_columns *columns;
	t_ast	**array = convert_to_array(data->tree);
	if (!array)
		exit(2); //manage error
	// sort !
	if (data->term.is_tty && data->color_parse_error)
		ft_putstr_fd("ft_ls: unparsable value for LS_COLORS environment variable\n", 2);
	if (ast_length(data->tree) > 1)
	{
		for (int i = 0; array[i]; i++)
		{
			if (array[i]->file_info.type == TYPE_DIR)
			{
				print_folder(array[i], data, 2);
				if (array[i + 1])
					fill_buff_char('\n');
			}
			else
			{
				if (data->flags.l || data->flags.g)
				{
					columns = parse_columns(array[i]);
					if (!columns)
						exit(2); // manage error
					print_file(array[i], data, columns);
					free(columns);
				}
				else
					print_file(array[i], data, NULL);
				if (data->flags.l)
					continue ;
				if (array[i + 1] && array[i + 1]->file_info.type != TYPE_DIR)
					fill_buff("  ", 2);
				else
					fill_buff("\n", 1);
			}
		}
	}
	else
	{
		if (data->tree->file_info.type == TYPE_DIR)
			print_folder(data->tree, data, 1);
		else
		{
			if (data->flags.l || data->flags.g)
			{
				columns = parse_columns(data->tree);
				if (!columns)
					exit(2); // manage error
				print_file(data->tree, data, columns);
				free(columns);
			}
			else
				print_file(data->tree, data, NULL);
			fill_buff_char('\n');
		}
	}
	free(array);
	flush();
}
