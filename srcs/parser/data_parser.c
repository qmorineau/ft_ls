#include "ft_ls.h"

int parse_file_infos(t_ast **node, t_flags flags)
{
	t_ast *current = *node;

	struct stat buff;

	if (stat(current->path, &buff) == 0)
	{
		char *file = ft_strrchr(current->path, '/');
		if (!file)
			current->file_info.name = ft_strdup(current->path);
		else
			current->file_info.name = ft_strdup(++file);
		// check name
			
		current->file_info.type = parse_file_type(&buff);
		if (flags.l || flags.g || 1) // opti no if else and assign all variable each time
		{
			// get everything
			current->file_info.size = buff.st_size;
			parse_permissions(&buff, &current->file_info.permissions);
		}
		else if (flags.t)
		{
			// get things to have time
		}
	}
	return (0);
}
int create_folder_data(t_ast **parent, t_flags flags);

int create_entry(t_ast **parent, char *begin_path, struct dirent *entry, t_flags flags)
{
	t_ast *tmp_ast;
	
	char *entry_path;
	if (!begin_path)
		entry_path = ft_strdup(entry->d_name);
	else
		entry_path = ft_strjoin(begin_path, entry->d_name);
	// get_attributes(entry_path);

	// check
	// int type = get_type(entry_path);
	// check
	tmp_ast = new_ast_node();
	//check
	tmp_ast->path = entry_path;
	parse_file_infos(&tmp_ast, flags);
	//check
	ast_addfront(&(*parent)->head, tmp_ast);

	if (flags.R && tmp_ast->file_info.type == TYPE_FOLDER)
	{
		create_folder_data(&tmp_ast, flags); //check res
	}

	return (0);
}

int create_folder_data(t_ast **parent, t_flags flags)
{
	t_ast *current = *parent;

	DIR* dir = opendir(current->path);
	char *path = ft_strjoin(current->path, "/");
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

int create_file_data(t_ast **parent, t_flags flags)
{
	t_ast *current = *parent;
	char *file = ft_strrchr(current->path, '/');
	if (!file)
		file = ft_strdup(current->path);

	char *path;

	if (strlen(current->path) - strlen(file) != 0)
		path = ft_strndup(current->path, strlen(current->path) - strlen(file));
	else
		path = NULL;
	
	char *current_folder;
	if (!path)
		current_folder = ft_strdup(".");
	else
		current_folder = ft_strjoin(path, "/.");

	DIR* dir = opendir(current_folder);

	struct dirent *entry = readdir(dir);
	while (entry)
	{
		if (!ft_strncmp(entry->d_name, file, strlen(entry->d_name) + 1))
			create_entry(parent, path, entry, flags);
		entry = readdir(dir);
	}
	free(file);
	free(path);
	free(current_folder);
	closedir(dir);
	return (0);
}

int parse_ast_node(t_ast **parent, t_flags flags)
{
	// parse_file_infos(parent, flags);
	switch ((*parent)->file_info.type)
	{
		case TYPE_FILE:
			create_file_data(parent, flags);
			break;
		case TYPE_FOLDER:
			create_folder_data(parent, flags);
			break;
	}
	return (0);
}

/* int parse_data(t_data *data)
{
	for (int i = 0; data->args[i]; i++)
	{
		int res = parse_ast_node(&data->args[i], data->flags);
		if (res == -1)
			return (-1);
	}
	return (0);
} */

/* int parse_arguments(int argc, char *argv[], t_data *data)
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
} */
