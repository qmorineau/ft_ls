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

// 0 ---
// 1 --x
// 2 -w-
// 3 -wx
// 4 r--
// 5 r-x
// 6 rw-
// 7 rwx

void print_file(t_ast *node, t_flags flags)
{
	if (flags.l)
	{
		t_file file = node->file_info;
		char buff[11] = "----------";
		buff[10] = 0;

		put_permissions(file, buff);
		ft_printf("%s user group %d time %s", buff, file.size, file.name);
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
		ft_printf("total %d\n", -1); // calculate total of memory block
	// sort the array from the flags
	print_list(node->head, flags);
}

void print_list(t_ast *head, t_flags flags)
{
	t_ast	**array = convert_to_array(head);

	sort_array(&array, test_ascii);

	for (int i = 0; array[i]; i++)
	{
		print_file(array[i], flags);
		if (array[i + 1])
		{
			if (flags.l)
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