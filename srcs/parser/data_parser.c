#include "ft_ls.h"

static t_ast *create_parent(int type)
{
	t_ast *new_parent = ft_calloc(1, sizeof(t_ast));
	if (!new_parent)
		return (NULL);
	new_parent->type = type;
	return (new_parent);
}

static int fill_file_struct(t_file *file, t_flags flags)
{
	(void) file;
	(void) flags;
	return (0);
}

static t_ast *parse_file(t_ast *parent, t_flags flags)
{
	t_ast *tmp = ft_calloc(1, sizeof(t_file));
	if (!tmp)
		return (NULL);
	fill_file_struct(tmp, flags); // check res
	t_list *node = ft_lstnew(tmp);
	if (!node)
		return (NULL);
	// add at the end of the ast list
	(void) parent;
	return 
}

static t_ast *parse_folder(t_ast *parent, t_flags flags)
{


	if (flags.R)
	{
		t_ast *tmp = parent->head;
		while (tmp)
		{
			parse_folder(tmp, flags);
			tmp = tmp->next;
		}
	}
}

int get_type(char *path)
{
	struct stat buff;

	if (stat(path, &buff) == 0)
	{
		switch (buff.st_mode & S_IFMT)
		{
			// Folder
			case S_IFDIR:
				return (TYPE_FOLDER);
			// File
			case S_IFREG:
				return (TYPE_FILE);
			// check man stat for more type
			default:
				// handle default ??
				return (-1);
		}
	}
	else
		return (-1);
}

int parse_data(t_data *data)
{
	for (int i = 0; data->args[i]; i++)
	{
		switch (data->args[i]->type)
		{
			case TYPE_FILE:
				parse_file(data->args[i], data->flags);
				break;
			case TYPE_FOLDER:
				parse_folder(data->args[i], data->flags);
				break;
		}
	}
	return (0);
}

int parse_arguments(int argc, char *argv[], t_data *data)
{
	int count = 0;

	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] != '-')
		{
			int type = get_type(argv[i]);
			switch (type)
			{
			case TYPE_FILE:
				data->args[count] = create_parent(TYPE_FILE);
				if (!data->args[count])
					return (-1);
				break;
			case TYPE_FOLDER:
				data->args[count] = create_parent(TYPE_FOLDER);
				if (!data->args[count])
					return (-1);
				break;
			default:
				// ERROR
				break;
			}
			data->args[count]->path = ft_strdup(argv[i]);
			if (!data->args[count]->path)
				return (-1);
			count++;
		}
	}
	return (0);
}