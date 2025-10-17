#include "ft_ls.h"

void print_list(t_ast *head, t_flags flags);

void print_file(t_ast *node, t_flags flags)
{
	if (flags.l)
	{
		t_file file = node->file_info;

		ft_printf("perm user group time %s", file.name);
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
	if (flags.R)
	{
		// print path
		if (print_path)
			ft_printf("%s:\n", node->path);
		if (flags.l)
			ft_printf("total %d\n", -1); // calculate total of memory block
	}
	// sort the array from the flags
	print_list(node->head, flags);
}

void print_list(t_ast *head, t_flags flags)
{
	t_ast	**array = convert_to_array(head);

	// sort data->args (it's an array)

	for (int i = 0; array[i]; i++)
	{
		print_file(array[i], flags);
		if (array[i + 1])
			write(1, "  ", 2);
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