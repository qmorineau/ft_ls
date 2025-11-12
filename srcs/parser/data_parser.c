#include "ft_ls.h"

int parse_file_infos(t_data *data, t_ast **node, int dir_fd)
{
	t_ast *current = *node;
	if (current->file_info.type == TYPE_LINK)
	{
		if (statx(dir_fd, get_name(&(*node)->file_info), AT_SYMLINK_NOFOLLOW, data->stax_mask , &current->file_info.sb))
		{
			current->file_info.error = errno;
			return (1);
		}
		current->file_info.redirect_file = parse_link(data);
		if (!current->file_info.redirect_file)
		{
			current->file_info.error = errno;
			data->exit_status = 2;
			return (0);
		}
	}
	else if (statx(dir_fd, get_name(&(*node)->file_info), AT_STATX_SYNC_AS_STAT, data->stax_mask , &current->file_info.sb))
	{
		current->file_info.error = errno;
		return (1);
	}
	if (!current->file_info.error)
	{
		if (!parse_file_from_stat(data, &current->file_info))
			return (0);
	}
	return (1);
}

static t_ast *create_entry(t_data *data, t_pool_ast **pool, struct dirent *entry, int dir_fd)
{
	t_ast *tmp_ast = get_new_ast(pool);
	if (!tmp_ast)
		return (NULL);	
	ft_strlcpy(tmp_ast->file_info.name.buff, entry->d_name, 256);
	tmp_ast->file_info.name_type = E_FILE;
	tmp_ast->file_info.type = dirent_type_parser(entry);
	push_path(data, &tmp_ast->file_info);
	if (!parse_file_infos(data, &tmp_ast, dir_fd))
	{
		pop_path(data);
		return NULL;
	}
	pop_path(data);
	return (tmp_ast);
}

static int parse_folder_entries(t_data *data, t_file *file, t_pool_ast **pool)
{
	struct dirent *entry;
	int dir_fd;

	DIR *dir = opendir(data->path);
	if (!dir) 
	{
		file->error = EACCES;
		print_error(data, *file);
		data->exit_status = 1;
		return (1);
	}
	dir_fd = dirfd(dir);
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' || (entry->d_name[0] == '.' && data->flags.a))
		{
			if (!create_entry(data, pool, entry, dir_fd))
				return (ast_pool_clear(pool), 0);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (1);
}

static int parse_folder_recursive(t_data *data, t_pool_ast *pool, t_ast **array)
{
	for (int i = 0; array[i]; i++)
	{
		if (array[i]->file_info.type == TYPE_DIR)
		{
			char *name = get_name(&array[i]->file_info);
			if (ft_strncmp("..", name, 3) && ft_strncmp(".", name, 2))
			{
				push_path(data, &array[i]->file_info);
				if (!parse_folder(data, &array[i]->file_info, 1))
					return (free(array), ast_pool_clear(&pool), 0);
				pop_path(data);
			}
		}
	}
	return (1);
}

int parse_folder(t_data *data, t_file *file, int is_header)
{
	t_pool_ast *pool = NULL;

	parse_folder_entries(data, file, &pool);
	t_ast **array = convert_to_array(pool);
	if (!array)
		return (ast_pool_clear(&pool), 0);
	sort_array(&array, data->flags);
	if (!ft_strchr(data->path, '/'))
		{ pop_path(data); push_path(data, file); }
	print_header(data, array, file, is_header);
	if (data->first_print)
		data->first_print = 0;
	if (data->flags.d)
		print_file(*file, data, NULL);
	else
	{
		t_columns columns;
		parse_columns(&columns, data, array);
		print_folder_files_list(data, array, &columns);
	}
	if (data->flags.R)
		parse_folder_recursive(data, pool, array);
	free(array);
	ast_pool_clear(&pool);
	return (1);
}
