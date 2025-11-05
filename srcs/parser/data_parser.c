#include "ft_ls.h"

void parse_file_infos(t_data *data, t_ast **node)
{
	t_ast *current = *node;

	if (lstat(data->path, &current->file_info.sb))
	{
		current->file_info.error = errno;
		return ;
	}
	if (current->file_info.type == TYPE_LINK)	
		current->file_info.redirect_file = parse_link(data); // check res // put it inside ????
	parse_file_from_stat(data, &current->file_info);
}

static t_ast *create_entry(t_data *data, t_pool_ast **pool, struct dirent *entry)
{
	t_ast *tmp_ast = get_new_ast(pool);
	
	ft_strlcpy(tmp_ast->file_info.name.buff, entry->d_name, 256);
	tmp_ast->file_info.name_type = E_FILE;
	tmp_ast->file_info.type = dirent_type_parser(entry);
	push_path(data, &tmp_ast->file_info);
	parse_file_infos(data, &tmp_ast);
	pop_path(data);
	return (tmp_ast);
}

void parse_folder(t_data *data, t_file *file)
{
	DIR *dir = opendir(data->path);
	if (!dir) return ;

	t_pool_ast *pool = NULL;
	struct dirent *entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' || (entry->d_name[0] == '.' && data->flags.a))
		{
			create_entry(data, &pool, entry);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	// sort entry
	t_ast **array = convert_to_array(pool);
	if (!array)
		exit(55); // manage error

	sort_array(&array, data->flags);
	// parse column if needed

	// Print dir header
	print_header(data, array, file, 1);
	t_columns columns;
	parse_columns(&columns, data, array);
	// print files
	if (data->flags.d)
		(void) data;
		// print_file(node, data, &columns);
	else
		print_folder_files_list(data, array, &columns);
	// print files
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
					parse_folder(data, &array[i]->file_info);
					pop_path(data);
				}
			}
		}
	}
	free(array);
	ast_pool_clear(&pool);
}
