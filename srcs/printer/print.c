#include "ft_ls.h"

void print_list(t_ast *head, t_data *data, t_columns *columns);

void print_str_columns(size_t columns_nbr, char *str)
{
	size_t str_len = ft_strlen(str);
	size_t space_nbr = columns_nbr - str_len;

	char buff[space_nbr + 1];

	ft_memset(buff, 32, space_nbr);
	buff[space_nbr] = 0;

	ft_printf("%s%s ", str, buff);
}

void print_minor_majora_columns(size_t columns_nbr, size_t minor, size_t major)
{
	char *str = ft_calloc(columns_nbr + 1, sizeof(char));
	// check res
	ft_memset(str, 32, columns_nbr);

	int i = columns_nbr - 1;

	while (major >= 10)
	{
		str[i--] = (major % 10) + 48;
		major /= 10;
	}
	if (i >= 0)
		str[i--] = major + 48;
	str[i--] = ' ';
	str[i--] = ',';
	while (minor >= 10)
	{
		str[i--] = (minor % 10) + 48;
		minor /= 10;
	}
	if (i >= 0)
		str[i] = minor + 48;
	ft_printf("%s ", str);
	free(str);
}

void print_size_t_columns(size_t columns_nbr, size_t nbr)
{
	char *str = ft_calloc(columns_nbr + 1, sizeof(char));
	// check res
	ft_memset(str, 32, columns_nbr);

	int i = columns_nbr - 1;

	while (nbr >= 10)
	{
		str[i--] = (nbr % 10) + 48;
		nbr /= 10;
	}
	if (i >= 0)
		str[i] = nbr + 48;
	ft_printf("%s", str);
	free(str);
}

void print_file_name(t_data *data, t_file *file)
{
	char *str;
	t_map *tmp = NULL;
	if (data->term.is_tty && !data->color_parse_error)
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
		ft_printf("\e[%sm%s\e[0m", str, file->name);
	}
	else
		ft_printf("%s", file->name);
}

void print_file(t_ast *node, t_data *data, t_columns *columns)
{
	if (data->flags.l || data->flags.g)
	{
		t_file file = node->file_info;

		ft_printf("%s ", file.permissions);
		print_size_t_columns(columns->link_max_len, file.link);
		write(1, " ", 1);
		if (!data->flags.g)
			print_str_columns(columns->user_max_len, file.user_name);
		print_str_columns(columns->group_max_len, file.group_name);
		if (file.type == TYPE_BLOCK || file.type == TYPE_CHR)
		{
			print_size_t_columns(columns->major_max_len, file.major);
			ft_printf(", ");
			print_size_t_columns(columns->minor_max_len, file.minor);
		}
		else
			print_size_t_columns(columns->size_max_len, file.size);
		write(1, " ", 1);
		if (data->flags.u)
			ft_printf("%s ", file.access_time);
		else
			ft_printf("%s ", file.mod_time);
		// ft_printf("%s", file.name);
		print_file_name(data, &file);
		if (file.type == TYPE_LINK)
		{
			ft_printf(" -> ");
			print_file_name(data, file.redirect_file);
		}
	}
	else
	{
		print_file_name(data, &node->file_info);
		// check collumns etc... bonus
	}
}

void print_folder(t_ast *node, t_data *data, int print_path)
{
	if ((print_path && data->flags.R && !data->flags.d) || print_path == 2)
		ft_printf("%s:\n", node->path);
	if (data->flags.l && !data->flags.d)
	{
		size_t blocks = 0;
		t_ast *tmp = node->head;
		while (tmp)
		{
			blocks += tmp->file_info.block_size;
			tmp = tmp->next;
		}
		ft_printf("total %d\n", blocks / 2); // total blocks of 512 bytes, need to show number of 1024 bytes blocks
	}
	if (data->flags.d)
	{
		t_columns *columns = parse_columns(node);
		//check res
		print_file(node, data, columns);
		write(1, "\n", 1);
		free(columns);
	}
	else
	{
		t_columns *columns = parse_columns(node);
		// check res
		print_list(node->head, data, columns);
		free(columns);
	}
}

void print_list(t_ast *head, t_data *data, t_columns *columns)
{
	if (!head)
		return ;

	t_ast	**array = convert_to_array(head);

	sort_array(&array, data->flags);
	// for (int i = 0; array[i]; i++)
	// {
	// 	printf("time = %zu\n", array[i]->file_info.raw_mod_time);
	// }
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
				write(1, "\n", 1);
				print_folder(array[i], data, 1);
			}
		}
	}
	free(array);
}

void print(t_data *data)
{	
	t_ast	**array = convert_to_array(data->tree);
	// sort
	int len = ast_length(data->tree);

	if (data->term.is_tty && data->color_parse_error)
		ft_printf("ft_ls: unparsable value for LS_COLORS environment variable\n");
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
					write(1, "\n", 1);
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