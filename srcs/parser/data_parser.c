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

int create_infos(t_ast **node, t_flags flags)
{
	t_ast *current = *node;

	struct stat buff;

	if (stat(current->path, &buff) == 0)
	{
		(void) flags;
		if (flags.l || flags.g) // opti no if else and assign all variable each time
		{
			// get everything
			current->file_info.size = buff.st_size;
			current->file_info.permissions = buff.st_mode & 07777; // Bits suppression to keep only permissions bits
		}
		else if (flags.t)
		{
			// get things to have time
		}
	}
	return (0);
}

int create_entry(t_ast *parent, char *begin_path, struct dirent *entry, t_flags flags)
{
	t_ast *tmp_ast;
	
	(void) flags;
	char *entry_path = ft_strjoin(begin_path, entry->d_name);
	get_attributes(entry_path);
	// check
	int type = get_type(entry_path);
	// check
	tmp_ast = new_ast_node(type);
	//check
	tmp_ast->path = entry_path;
	tmp_ast->file_info.name = ft_strdup(entry->d_name);
	create_infos(&tmp_ast, flags);
	//check
	ast_addfront(&parent->head, tmp_ast);

	return (0);
}

int create_ast_list(t_ast *parent, t_flags flags)
{
	(void) flags;

	DIR* dir = opendir(parent->path);
	char *path = ft_strjoin(parent->path, "/");
	// check res

	struct dirent *entry = readdir(dir);
	// check res
	while (entry)
	{
		if (entry->d_name[0] == '.')
		{
			if (flags.a)
				create_entry(parent, path, entry, flags);
		}
		else
			create_entry(parent, path, entry, flags);
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
