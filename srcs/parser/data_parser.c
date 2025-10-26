#include "ft_ls.h"

int parse_file_infos(t_ast **node, t_flags flags)
{
	t_ast *current = *node;

	struct stat buff;

	int res;
	if (current->file_info.type == TYPE_LINK)
		res = lstat(current->path, &buff);
	else
		res = stat(current->path, &buff);
	if (res)
		return (1);
	
	if (flags.l || flags.g || 1) // opti no if else and assign all variable each time
	{
		if (parse_file_from_stat(&current->file_info, &buff, current->path))
			exit(1); // error?
	}
	else if (flags.t)
	{
		// get things to have time
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
	tmp_ast = new_ast_node();
	//check
	tmp_ast->path = entry_path;
	ft_strlcpy(tmp_ast->file_info.name, entry->d_name, 256);
	tmp_ast->file_info.type = dirent_type_parser(entry);
	parse_file_infos(&tmp_ast, flags);
	//check
	ast_addback(&(*parent)->head, tmp_ast);

	if (flags.R && tmp_ast->file_info.type == TYPE_DIR)
	{
		create_folder_data(&tmp_ast, flags); //check res
	}

	return (0);
}

int create_folder_data(t_ast **parent, t_flags flags)
{
	// if (flags.d)
	// 	return (0);
	t_ast *current = *parent;

	DIR* dir = opendir(current->path);
	// check res ??
	char *path = ft_strjoin(current->path, "/");
	// check res

	struct dirent *entry = readdir(dir);
	// check res
	while (entry)
	{
		if (entry->d_name[0] == '.')
		{
			if (flags.a || flags.f)
			{
				if (!flags.d || (flags.d && entry->d_type == TYPE_DIR))
					create_entry(parent, path, entry, flags);
			}
		}
		else if (!flags.d || (flags.d && entry->d_type == TYPE_DIR))
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
	switch ((*parent)->file_info.type)
	{
		case TYPE_FILE:
			create_file_data(parent, flags);
			break;
		case TYPE_DIR:
			create_folder_data(parent, flags);
			break;
	}
	return (0);
}
