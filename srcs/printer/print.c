#include "ft_ls.h"

void print_list(t_ast *head, t_flags flags);

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

void print_file(t_ast *node, t_flags flags)
{
	if (flags.l || flags.g)
	{
		t_file file = node->file_info;
		char buff[11] = "----------";
		buff[10] = 0;

		put_permissions(file, buff);

		if (flags.g)
			ft_printf("%s %u %s %d %s %s", buff, file.link, file.group_name, file.size, file.time,file.name);
		else
			ft_printf("%s %u %s %s %d %s %s", buff, file.link, file.user_name, file.group_name, file.size, file.time, file.name);
		// long listing print
	}
	else
	{
		// print name
		ft_printf("%s", node->file_info.name);
		// check collumns etc...
	}
}

void print_folder(t_ast *node, t_flags flags, int print_path)
{
	// print path
	if (print_path && flags.R)
		ft_printf("%s:\n", node->path);
	if (flags.l)
	{
		int blocks = 0;
		t_ast *tmp = node->head;
		while (tmp)
		{
			if (tmp->file_info.size)
			{
				int res = (tmp->file_info.size / 1024) + 1;
				blocks += res;
				printf("res = %d\nsize = %zu\n", res, tmp->file_info.size);
			}
			tmp = tmp->next;
		}
		ft_printf("total %d\n", blocks); // calculate total of memory block
	}
	if (flags.d)
	{
		print_file(node, flags);
		write(1, "\n", 1);
	}
	else
		print_list(node->head, flags);
}

void print_list(t_ast *head, t_flags flags)
{
	if (!head)
		return ;

	t_ast	**array = convert_to_array(head);

	sort_array(&array, flags);

	for (int i = 0; array[i]; i++)
	{
		print_file(array[i], flags);
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
				print_folder(array[i], flags, 1);
		}
	}
	write(1, "\n", 1);
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
				print_file(array[i], data->flags);
		}
	}
	else
	{
		if (data->tree->file_info.type == TYPE_FOLDER)
			print_folder(data->tree, data->flags, 1);
		else
			print_file(data->tree, data->flags);
	}
	free(array);
}