#include "ft_ls.h"

void parse_file_infos(t_data *data, t_ast **node)
{
	t_ast *current = *node;

	if (data->flags.l || data->flags.u || data->flags.g || data->flags.t) // remove this if, call it before calling parse_file_infos
	{
		if (lstat(current->path, &current->file_info.sb))
		{
			current->file_info.error = errno;
			return ;
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
	}
	if (current->file_info.type == TYPE_LINK)
		current->file_info.redirect_file = parse_link(&current->file_info.sb, current->path); // check res // put it inside ????
}
static void create_folder_data(t_data *data, t_ast **parent);

static t_ast *create_entry(t_data *data, t_pool_ast *pool, struct dirent *entry)
{
	t_ast *tmp_ast = get_new_ast(&data->pools);
	
	// tmp_ast->file_info.acl_char = get_acl(entry_path);
	// tmp_ast->path = entry_path;
	ft_strlcpy(tmp_ast->file_info.name.buff, entry->d_name, 256);
	tmp_ast->file_info.type = dirent_type_parser(entry);
	return (tmp_ast);
	ast_addback(&(*parent)->head, tmp_ast);
	parse_file_infos(data, &tmp_ast);
	return (0);
}

static void create_folder_data(t_data *data, t_ast **parent)
{
	t_pool_ast *dir_pool = NULL;

	char path[PATH_MAX];

	t_ast *current = *parent;
	t_flags flags = data->flags;

	DIR* dir = opendir(current->path);
	if (!dir)
	{
		current->file_info.error = errno;
		return ;
	}
	ft_strlcpy(path, current->path, PATH_MAX);
	path[(*parent)->path_len] = 0;

	struct dirent *entry = readdir(dir);

	while (entry)
	{
		if (entry->d_name[0] == '.')
		{
			if (flags.a && (!flags.d || (flags.d && entry->d_type == TYPE_DIR)))
				create_entry(data, parent, path, entry);
		}
		else if (!flags.d || (flags.d && entry->d_type == TYPE_DIR))
			create_entry(data, parent, path, entry);
		entry = readdir(dir);
	}
	closedir(dir);
}

void create_file_data(t_data *data, t_ast **parent)
{
	char path[PATH_MAX];
	char file[FILENAME_MAX];

	t_ast *current = *parent;
	// printf("current path = %s\n", current->path);
	char *file_name = ft_strrchr(current->path, '/');
	if (!file_name)
		ft_strlcpy(file, current->path, FILENAME_MAX);
	else
		ft_strlcpy(file, file_name, FILENAME_MAX);

	size_t file_len = ft_strlen(file);
	if (current->path_len - file_len != 0)
	{
		size_t len = ft_strlcpy(path, current->path, PATH_MAX);
		path[len - file_len] = 0;
	}
	else
		ft_strlcpy(path, ".", PATH_MAX);

	DIR* dir = opendir(path);
	if (!dir)
	{
		current->file_info.error = errno;
		return ;
	}

	struct dirent *entry = readdir(dir);
	while (entry)
	{
		if (!ft_strncmp(entry->d_name, file, ft_strlen(entry->d_name) + 1))
			create_entry(data, parent, path, entry);
		entry = readdir(dir);
	}
	closedir(dir);
}

int parse_ast_node(t_data *data, t_ast **parent)
{
	// printf("parse_ast_node %s = %d\n", get_name(&(*parent)->file_info), (*parent)->file_info.type);
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
