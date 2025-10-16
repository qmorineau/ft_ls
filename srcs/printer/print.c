#include "ft_ls.h"

void print_entry(t_ast *node, t_flags flags)
{
	// print file

	// if folder, print path then sort file then print each element
	if (flags.R)
	{
		// print path
		ft_printf("%s:\n", node->path);
		if (flags.l)
			ft_printf("total -1\n"); // calculate total of memory block
	}
	if (flags.l)
	{
		
		// long listing print
	}
	else
	{
		// print name
		ft_printf("%s")
		// check collumns etc...
	}
}

void print(t_data *data)
{
	// sort data->args (it's an array)
	for (int i = 0; data->args[i]; i++)
	{
		print_entry(data->args[i], data->flags);
	}
}