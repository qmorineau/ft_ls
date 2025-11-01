#include "ft_ls.h"

void parse_file_infos(t_data *data, t_ast **node)
{
	t_ast *current = *node;

	if (data->flags.l || data->flags.u || data->flags.g || data->flags.t)
	{
		int res = lstat(current->path, &current->file_info.sb);
		if (res)
		{
			current->file_info.error = STAT_ERROR;
			return ;
		}
	}
	
	if (data->flags.l || data->flags.g)
		parse_file_from_stat(data, &current->file_info);
	else if (data->flags.u)
	{
		current->file_info.time = current->file_info.sb.st_atime;
		if (parse_time(data, &current->file_info))
			free_all_and_exit(&data, 2);
	}
	else if (data->flags.t)
	{
		current->file_info.time = current->file_info.sb.st_mtime;
		if (parse_time(data, &current->file_info))
			free_all_and_exit(&data, 2);
	}
	if (current->file_info.type == TYPE_LINK)
		current->file_info.redirect_file = parse_link(&current->file_info.sb, current->path); // check res
}
static void create_folder_data(t_data *data, t_ast **parent);

static int create_entry(t_data *data, t_ast **parent, char *begin_path, struct dirent *entry)
{
	t_ast *tmp_ast;
	
	char *entry_path;
	if (!begin_path)
		entry_path = entry->d_name;
	else
		entry_path = ft_strjoin(begin_path, entry->d_name);
	if (!entry_path)
		free_all_and_exit(&data, 2);
	tmp_ast = get_new_ast(&data->pools);
	tmp_ast->file_info.acl_char = get_acl(entry_path);
	tmp_ast->path = entry_path;
	ft_strlcpy(tmp_ast->file_info.name.buff, entry->d_name, 256);
	tmp_ast->file_info.type = dirent_type_parser(entry);
	ast_addback(&(*parent)->head, tmp_ast);
	parse_file_infos(data, &tmp_ast);

	if (data->flags.R && tmp_ast->file_info.type == TYPE_DIR)
	{
		if (strncmp("..", get_name(&tmp_ast->file_info), 3) && strncmp(".", get_name(&tmp_ast->file_info), 2))
			create_folder_data(data, &tmp_ast);
	}
	return (0);
}

static void create_folder_data(t_data *data, t_ast **parent)
{
	t_ast *current = *parent;
	t_flags flags = data->flags;

	DIR* dir = opendir(current->path);
	if (!dir)
	{
		current->file_info.error = OPENDIR_ERROR;
		return ;
	}
	char *path = ft_strjoin(current->path, "/");
	if (!path)
	{
		closedir(dir);
		free_all_and_exit(&data, 2);
	}
	struct dirent *entry = readdir(dir);
	if (!entry)
	{
		closedir(dir);
		free(path);
		free_all_and_exit(&data, 2);
	}
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
}

void create_file_data(t_data *data, t_ast **parent)
{
	t_ast *current = *parent;
	char *file = ft_strrchr(current->path, '/');
	if (!file)
		file = ft_strdup(current->path);
	else
		file = ft_strdup(file);
	if (!file)
		free_all_and_exit(&data, 2);

	char *path;
	if (strlen(current->path) - ft_strlen(file) != 0)
	{
		path = ft_strndup(current->path, ft_strlen(current->path) - ft_strlen(file));
		if (!path)
			free_all_and_exit(&data, 2);
	}
	else
		path = NULL;
	
	char *current_folder;
	if (!path)
		current_folder = ft_strdup(".");
	else
		current_folder = ft_strjoin(path, "/.");
	if (!current_folder)
	{
		if (path)
			free(path);
		free_all_and_exit(&data, 2);
	}

	DIR* dir = opendir(current_folder);
	if (!dir)
	{
		current->file_info.error = OPENDIR_ERROR;
		return ;
	}

	struct dirent *entry = readdir(dir);
	while (entry)
	{
		if (!ft_strncmp(entry->d_name, file, ft_strlen(entry->d_name) + 1))
			create_entry(data, parent, path, entry);
		entry = readdir(dir);
	}
	free(file);
	free(path);
	free(current_folder);
	closedir(dir);
}

int parse_ast_node(t_data *data, t_ast **parent)
{
	switch ((*parent)->file_info.type)
	{
		case TYPE_DIR:
			create_folder_data(data, parent);
			break;
		default:
			create_file_data(data, parent);
			break;
	}
	return (0);
}
