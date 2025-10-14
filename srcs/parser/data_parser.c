#include "ft_ls.h"

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

int create_ast_list(t_ast *parent, t_flags flags)
{
	(void) flags;

	t_ast *tmp_ast;
	DIR* dir = opendir(parent->path);
	char *path = ft_strjoin(parent->path, "/");
	// check res

	struct dirent *entry = readdir(dir);
	// check res
	while (entry)
	{
		char *entry_path = ft_strjoin(path, entry->d_name);
		// check
		int type = get_type(entry_path);
		// check
		tmp_ast = new_ast_node(type);
		//check
		tmp_ast->path = entry_path;
		ast_addfront(&parent->head, tmp_ast);
		entry = readdir(dir);
	}
	closedir(dir);
	free(path);
	return (0);
}

int parse_ast_node(t_ast *parent, t_flags flags)
{
	switch (get_type(parent->path))
	{
		case TYPE_FILE:
			/* code */
			break;
		case TYPE_FOLDER:
			create_ast_list(parent, flags);
			break;
	}
	return (0);
}

int parse_data(t_data *data)
{
	for (int i = 0; data->args[i]; i++)
	{
		int res = parse_ast_node(data->args[i], data->flags);
		if (res == -1)
			return (-1);
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
					data->args[count] = new_ast_node(TYPE_FILE);
					if (!data->args[count])
						return (-1);
					break;
				case TYPE_FOLDER:
					data->args[count] = new_ast_node(TYPE_FOLDER);
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