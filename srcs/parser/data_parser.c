#include "ft_ls.h"

int parse_file_infos(t_data *data, t_ast **node, int dir_fd)
{
	t_ast *current = *node;
	// printf("path = %s\n", data->path);
	// printf("name = %s\n", get_name(&(*node)->file_info));
	if (current->file_info.type == TYPE_LINK)
	{
		if (statx(dir_fd, get_name(&(*node)->file_info), AT_SYMLINK_NOFOLLOW, data->stax_mask , &current->file_info.sb))
		{
			current->file_info.error = errno;
			return (1);
		}
		// perror("A");
		current->file_info.redirect_file = parse_link(data);
		if (!current->file_info.redirect_file)
		{
			current->file_info.error = errno;
			data->exit_status = 2;
			return (0);
		}
		// perror("B");
		// fprintf(stderr, "error = %d\n", current->file_info.error);
	}
	else if (statx(dir_fd, get_name(&(*node)->file_info), AT_STATX_SYNC_AS_STAT, data->stax_mask , &current->file_info.sb))
	{
		current->file_info.error = errno;
		return (1);
	}
	if (!current->file_info.error)
		parse_file_from_stat(data, &current->file_info);
	return (1);
}

static t_ast *create_entry(t_data *data, t_pool_ast **pool, struct dirent *entry, int dir_fd)
{
	t_ast *tmp_ast = get_new_ast(pool);
	
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

int parse_folder(t_data *data, t_file *file, int is_header)
{
	// fprintf(stderr, "OPENDIR = %s\n", data->path);
	DIR *dir = opendir(data->path);
	if (!dir) 
	{
		file->error = EACCES;
		print_error(data, *file);
		data->exit_status = 1;
		return (1);
	}
	int dir_fd = dirfd(dir);
	// Parse Dir
	t_pool_ast *pool = NULL;
	struct dirent *entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' || (entry->d_name[0] == '.' && data->flags.a))
			create_entry(data, &pool, entry, dir_fd);
		entry = readdir(dir);
	}
	closedir(dir);
	// To array and sort
	t_ast **array = convert_to_array(pool);
	if (!array)
		return (ast_pool_clear(&pool), 0);
	sort_array(&array, data->flags);
	// Header
	if (!ft_strchr(data->path, '/'))
	{
		pop_path(data);
		push_path(data, file);
	}
	print_header(data, array, file, is_header);
	if (data->first_print)
		data->first_print = 0;
	// Print
	if (data->flags.d)
		print_file(*file, data, NULL);
	else
	{
		// Columns
		t_columns columns;
		parse_columns(&columns, data, array);
		print_folder_files_list(data, array, &columns);
	}
	// Recursive
	if (data->flags.R)
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
	}
	free(array);
	ast_pool_clear(&pool);
	return (1);
}
