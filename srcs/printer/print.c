#include "ft_ls.h"

void print_list(t_ast *head, t_flags flags, t_columns *data);

void put_permissions(t_file file, char buff[11])
{
	if (file.type == TYPE_FOLDER)
		buff[0] = 'd';
	for (int i = 0; i < 3; i++)
	{
		int nbr = file.permissions[i] - 48;
		if (nbr >= 4)
		{
			nbr -= 4;
			buff[3 * i + 1] = 'r';
		}
		if (nbr >= 2)
		{
			nbr -= 2;
			buff[3 * i + 2] = 'w';
		}
		if (nbr >= 1)
			buff[3 * i + 3] = 'x';
	}
}

void print_str_columns(size_t columns_nbr, char *str)
{
	size_t str_len = ft_strlen(str);
	size_t space_nbr = columns_nbr - str_len;

	char buff[space_nbr + 1];

	ft_memset(buff, 32, space_nbr);
	buff[space_nbr] = 0;

	ft_printf("%s%s ", str, buff);
}

void print_size_t_columns(size_t columns_nbr, size_t nbr)
{
	printf("columns nbr = %zu\n", columns_nbr);
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
	ft_printf("%s ", str);
	free(str);
}

void print_file(t_ast *node, t_flags flags, t_columns *data)
{
	if (flags.l || flags.g)
	{
		t_file file = node->file_info;
		char buff[11] = "----------";
		buff[10] = 0;

		put_permissions(file, buff);

		ft_printf("%s %u ", buff, file.link);
		if (!flags.g)
			print_str_columns(data->user_max_len, file.user_name);
		print_str_columns(data->group_max_len, file.group_name);
		print_size_t_columns(data->size_max_len, file.size);
		if (flags.u)
			ft_printf("%s ", file.access_time);
		else
			ft_printf("%s ", file.time);
		ft_printf("%s", file.name);
	}
	else
	{
		ft_printf("%s", node->file_info.name);
		// check collumns etc... bonus
	}
}

void print_folder(t_ast *node, t_flags flags, int print_path)
{
	if (print_path && flags.R)
		ft_printf("%s:\n", node->path);
	if (flags.l && !flags.d)
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
	if (flags.d)
	{
		t_columns *data = parse_columns(node);
		//check res
		print_file(node, flags, data);
		write(1, "\n", 1);
		free(data);
	}
	else
	{
		t_columns *data = parse_columns(node);
		// check res
		print_list(node->head, flags, data);
		free(data);
	}
	// write(1, "\n", 1);
}

void print_list(t_ast *head, t_flags flags, t_columns *data)
{
	if (!head)
		return ;

	t_ast	**array = convert_to_array(head);

	sort_array(&array, flags);
	// for (int i = 0; array[i]; i++)
	// {
	// 	printf("time = %zu\n", array[i]->file_info.raw_time);
	// }
	for (int i = 0; array[i]; i++)
	{
		print_file(array[i], flags, data);
		if (array[i + 1])
		{
			if (flags.l || flags.g)
				write(1, "\n", 1);
			else
				write(1, "  ", 2);
		}
	}
	write(1, "\n", 1);
	if (flags.R)
	{
		for (int i = 0; array[i]; i++)
		{
			if (array[i]->file_info.type == TYPE_FOLDER)
			{
				write(1, "\n", 1);
				print_folder(array[i], flags, 1);
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

	if (len > 1)
	{
		for (int i = 0; array[i]; i++)
		{
			if (array[i]->file_info.type == TYPE_FOLDER)
				print_folder(array[i], data->flags, 0);
			else
				print_file(array[i], data->flags, NULL);
		}
	}
	else
	{
		if (data->tree->file_info.type == TYPE_FOLDER)
			print_folder(data->tree, data->flags, 1);
		else
			print_file(data->tree, data->flags, NULL);
	}
	free(array);
}