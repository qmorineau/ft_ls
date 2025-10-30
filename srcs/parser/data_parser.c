#include "ft_ls.h"

void parse_file_infos(t_data *data, t_ast **node)
{
	t_ast *current = *node;

	struct stat buff;

	int res;
	if (current->file_info.type == TYPE_LINK)
		res = lstat(current->path, &buff);
	else
		res = stat(current->path, &buff);
	if (res)
	{
		current->file_info.stat_error = 1;
		return ;
	}
	
	if (data->flags.l || data->flags.g)
	{
		if (parse_file_from_stat(data, &current->file_info, &buff, current->path))
			exit(1); // error?
	}
	else if (data->flags.t)
	{
		current->file_info.raw_mod_time = buff.st_atime;
		if (parse_modified_time(data, &current->file_info, &buff))
			exit(1); // manage error
	}
}
int create_folder_data(t_data *data, t_ast **parent);

static int create_entry(t_data *data, t_ast **parent, char *begin_path, struct dirent *entry)
{
	t_ast *tmp_ast;
	
	char *entry_path;
	if (!begin_path)
		entry_path = ft_strdup(entry->d_name);
	else
		entry_path = ft_strjoin(begin_path, entry->d_name);
	//check res

	// check
	tmp_ast = new_ast_node();
	//check
	tmp_ast->file_info.acl_char = get_acl(entry_path);
	tmp_ast->path = entry_path;
	ft_strlcpy(tmp_ast->file_info.name, entry->d_name, 256);
	tmp_ast->file_info.type = dirent_type_parser(entry);
	parse_file_infos(data, &tmp_ast);
	ast_addback(&(*parent)->head, tmp_ast);

	if (data->flags.R && tmp_ast->file_info.type == TYPE_DIR)
	{
		if (strncmp("..", tmp_ast->file_info.name, 3) && strncmp(".", tmp_ast->file_info.name, 2))
			create_folder_data(data, &tmp_ast); //check res
	}
	return (0);
}

int create_folder_data(t_data *data, t_ast **parent)
{
	t_ast *current = *parent;
	t_flags flags = data->flags;

	DIR* dir = opendir(current->path);
	if (!dir)
	{
		ft_putstr_fd("ft_ls: cannot open directory '", 2);
		ft_putstr_fd(current->path, 2);
		perror("'");
		return (0);
	}
	char *path = ft_strjoin(current->path, "/");
	if (!path)
		exit(2); // manage error
	struct dirent *entry = readdir(dir);
	if (!entry)
		exit(50); // manage error
	while (entry)
	{
		if (entry->d_name[0] == '.')
		{
			if (flags.a)
			{
				if (!flags.d || (flags.d && entry->d_type == TYPE_DIR))
					create_entry(data, parent, path, entry);
			}
		}
		else if (!flags.d || (flags.d && entry->d_type == TYPE_DIR))
			create_entry(data, parent, path, entry);
		entry = readdir(dir);
	}
	closedir(dir);
	free(path);
	return (0);
}

int create_file_data(t_data *data, t_ast **parent)
{
	t_ast *current = *parent;
	char *file = ft_strrchr(current->path, '/');
	if (!file)
		file = ft_strdup(current->path);
	else
		file = ft_strdup(file);
	if (!file)
		exit(2); // manage error

	char *path;
	if (strlen(current->path) - ft_strlen(file) != 0)
	{
		path = ft_strndup(current->path, ft_strlen(current->path) - ft_strlen(file));
		if (!path)
			exit(2); // manage error
	}
	else
		path = NULL;
	
	char *current_folder;
	if (!path)
		current_folder = ft_strdup(".");
	else
		current_folder = ft_strjoin(path, "/.");
	if (!current_folder)
		exit(2); // manage error

	DIR* dir = opendir(current_folder);
	if (!opendir)
		return (1); // manage error

	struct dirent *entry = readdir(dir);
	if (!entry)
		exit(2); // manage error
	while (entry)
	{
		if (!ft_strncmp(entry->d_name, file, ft_strlen(entry->d_name) + 1))
			create_entry(data, parent, path, entry);
		entry = readdir(dir);
		if (!entry)
			exit(2); // manage error
	}
	free(file);
	free(path);
	free(current_folder);
	closedir(dir);
	return (0);
}

int parse_ast_node(t_data *data, t_ast **parent)
{
	switch ((*parent)->file_info.type)
	{
		case TYPE_DIR:
			create_folder_data(data, parent);
			// check res ?
			break;
		default:
			create_file_data(data, parent);
			// check res ?
			break;
	}
	return (0);
}
