#include "ft_ls.h"

int parse_file_infos(t_data *data, t_ast **node)
{
	t_ast *current = *node;

	if (lstat(data->path, &current->file_info.sb))
	{
		current->file_info.error = errno;
		return (1);
	}
	if (current->file_info.type == TYPE_LINK)
	{
		current->file_info.redirect_file = parse_link(data);
		if (!current->file_info.redirect_file)
		{
			data->exit_status = 2;
			return (0);
		}
	}
	if (!current->file_info.error)
		parse_file_from_stat(data, &current->file_info);
	return (1);
}

static t_ast *create_entry(t_data *data, t_pool_ast **pool, struct dirent *entry)
{
	t_ast *tmp_ast = get_new_ast(pool);
	
	ft_strlcpy(tmp_ast->file_info.name.buff, entry->d_name, 256);
	tmp_ast->file_info.name_type = E_FILE;
	tmp_ast->file_info.type = dirent_type_parser(entry);
	push_path(data, &tmp_ast->file_info);
	if (!parse_file_infos(data, &tmp_ast))
		return NULL;
	pop_path(data);
	return (tmp_ast);
}

int parse_folder(t_data *data, t_file *file, int is_header)
{
	DIR *dir = opendir(data->path);
	if (!dir) 
	{
		file->error = EACCES;
		data->exit_status = 1;
		return (1);
	}
	// Parse Dir
	t_pool_ast *pool = NULL;
	struct dirent *entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' || (entry->d_name[0] == '.' && data->flags.a))
			create_entry(data, &pool, entry);
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
	// Columns
	t_columns columns;
	parse_columns(&columns, data, array);
	// Print
	if (data->flags.d)
		print_file(*file, data, &columns);
	else
		print_folder_files_list(data, array, &columns);
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
