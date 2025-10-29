#include "ft_ls.h"

static void print_list(t_ast *head, t_data *data, t_columns *columns);

static void print_str_columns(size_t columns_nbr, char *str)
{
	size_t str_len = ft_strlen(str);
	size_t space_nbr = columns_nbr - str_len;

	char buff[space_nbr + 1];

	ft_memset(buff, 32, space_nbr);
	buff[space_nbr] = 0;

	ft_putstr_fd(str, 1);
	ft_putstr_fd(buff, 1);
	write(1, " ", 1);
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
	ft_putstr_fd(str, 1);
	free(str);
}

static void print_file_name(t_data *data, t_file *file)
{
	char *str;
	t_map *tmp = NULL;
	if (!data->term.is_tty || data->color_parse_error /* || 1 */) // remove 1
		ft_putstr_fd(file->name, 1);
	else
	{
		switch (file->type)
		{
			case TYPE_FILE:
				if (strchr(file->permissions, 'S'))
				{
					if (file->permissions[3] == 'S')
						tmp = map_get(data->colors, "su");
					else
						tmp = map_get(data->colors, "sg");
				}
				else if (strchr(file->permissions, 'x'))
					tmp = map_get(data->colors, "ex");
				else 
				{
					tmp = find_extension(data->file_colors, file->name);
					if (!tmp)
						tmp = map_get(data->colors, "fi");
				}
				break;
			case TYPE_DIR:
				if (strchr(file->permissions, 't'))
					tmp = map_get(data->colors, "ow");
				else
					tmp = map_get(data->colors, "di");
				break;
			case TYPE_LINK:
				if (file->redirect_file->type == TYPE_BROKEN_LINK)
					tmp = map_get(data->colors, "or");
				else
					tmp = map_get(data->colors, "ln");
				break;
			case TYPE_BROKEN_LINK:
				tmp = map_get(data->colors, "or");
				break;
			case TYPE_BLOCK:
				tmp = map_get(data->colors, "bd");
				break;
			case TYPE_PIPE:
				tmp = map_get(data->colors, "pi");
				break;
			case TYPE_SOCKET:
				tmp = map_get(data->colors, "so");
				break;
			case TYPE_CHR:
				tmp = map_get(data->colors, "cd");
				break;
		}
		if (tmp)
			str = tmp->value;
		else
			str = "0";
		ft_putstr_fd("\e[", 1);
		ft_putstr_fd(str, 1);
		ft_putstr_fd("m", 1);
		ft_putstr_fd(file->name, 1);
		ft_putstr_fd("\e[0m", 1);
	}
}

static void print_file(t_ast *node, t_data *data, t_columns *columns)
{
	if (data->flags.l || data->flags.g)
	{
		t_file file = node->file_info;

		ft_putstr_fd(file.permissions, 1);
		if (columns->as_acl)
			write(1, &file.acl_char, 1);
		write(1, " ", 1);
		print_size_t_columns(columns->link_max_len, file.link);
		write(1, " ", 1);
		if (!data->flags.g)
			print_str_columns(columns->user_max_len, file.user_name);
		print_str_columns(columns->group_max_len, file.group_name);
		if (file.type == TYPE_BLOCK || file.type == TYPE_CHR)
		{
			print_size_t_columns(columns->major_max_len, file.major);
			write(1, ", ", 2);
			print_size_t_columns(columns->minor_max_len, file.minor);
		}
		else
			print_size_t_columns(columns->size_max_len, file.size);
		write(1, " ", 1);
		if (data->flags.u)
			ft_putstr_fd(file.access_time, 1);
		else
			ft_putstr_fd(file.mod_time, 1);
		write(1, " ", 1);
		print_file_name(data, &file);
		if (file.type == TYPE_LINK)
		{
			write(1, " -> ", 4);
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
		ft_putstr_fd(node->path, 1);
		ft_putstr_fd(":\n", 1);
	}
	if (node->file_info.stat_error)
	{
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
		if (node->file_info.stat_error)
		{
			ft_putstr_fd("total ", 1);
			ft_printf("%d\n", blocks / 2);
		}
	}
	t_columns *columns = parse_columns(node);
	if (!columns)
		exit(2); // manage error
	if (data->flags.d)
	{
		print_file(node, data, columns);
		write(1, "\n", 1);
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
				write(1, "\n", 1);
			else
				write(1, "  ", 2);
		}
	}
	write(1, "\n", 1);
	if (data->flags.R)
	{
		for (int i = 0; array[i]; i++)
		{
			if (array[i]->file_info.type == TYPE_DIR)
			{
				if (strncmp("..", array[i]->file_info.name, 3) && strncmp(".", array[i]->file_info.name, 2))
				{
					write(1, "\n", 1);
					print_folder(array[i], data, 1);
				}	
			}
		}
	}
	free(array);
}

void print(t_data *data)
{	
	t_ast	**array = convert_to_array(data->tree);
	if (!array)
		exit(2); //manage error

	// sort ?

	int len = ast_length(data->tree);

	if (data->term.is_tty && data->color_parse_error)
		ft_putstr_fd("ft_ls: unparsable value for LS_COLORS environment variable\n", 1);
	if (len > 1)
	{
		for (int i = 0; array[i]; i++)
		{
			if (array[i]->file_info.type == TYPE_DIR)
			{
				print_folder(array[i], data, 2);
				if (array[i + 1])
					write(1, "\n", 1);
			}
			else
			{
				print_file(array[i], data, NULL);
				if (data->flags.l)
					continue ;
				if (array[i + 1] && array[i + 1]->file_info.type != TYPE_DIR)
					write(1, "  ", 2);
				else
					write(1, "\n\n", 2);
			}
		}
	}
	else
	{
		if (data->tree->file_info.type == TYPE_DIR)
			print_folder(data->tree, data, 1);
		else
		{
			print_file(data->tree, data, NULL);
			write(1, "\n", 1);
		}
	}
	free(array);
}