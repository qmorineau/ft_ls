#include "ft_ls.h"

static void parse_arg_type(t_data *data, t_ast *new_node)
{
	push_path(data, &new_node->file_info);
	if (statx(AT_FDCWD, data->path, AT_STATX_SYNC_AS_STAT, data->stax_mask , &new_node->file_info.sb) == 0)
		new_node->file_info.type = stat_type_parser(&new_node->file_info.sb);
	else if (errno == EPERM || errno == EACCES)
	{
		if (statx(AT_FDCWD, data->path, AT_SYMLINK_NOFOLLOW, data->stax_mask , &new_node->file_info.sb) == 0)
			new_node->file_info.type = stat_type_parser(&new_node->file_info.sb);
		else
		{
			data->exit_status = 2;
			new_node->file_info.error = ENOENT;
		}
	}
	else
	{
		data->exit_status = 2;
		new_node->file_info.error = ENOENT;
	}
	if (new_node->file_info.error)
		print_error(data, new_node->file_info);
	pop_path(data);
}

static int parse_arg(t_data *data, t_ast *new_node, int array_len)
{
	push_path(data, &new_node->file_info);
	if (data->flags.d && !new_node->file_info.error)
		parse_file_from_stat(data, &new_node->file_info);
	if (new_node->file_info.type == TYPE_DIR && !data->flags.d)
	{
		if (array_len == 1 && !data->flags.R)
			{if (!parse_folder(data, &new_node->file_info, 0)) return (0);}
		else
			{if (!parse_folder(data, &new_node->file_info, 2)) return (0);}
	}
	else
	{
		if (!parse_file_infos(data, &new_node, AT_FDCWD))
			return (0);
		if (!new_node->file_info.error)
			print_file(new_node->file_info, data, NULL);
		if (data->first_print)
			data->first_print = 0;
	}
	pop_path(data);
	return (1);
}

int parse_root(t_data *data, t_pool_ast *args_pool)
{
	t_ast *new_node = get_new_ast(&args_pool);
	if (!new_node)
		return (0);
	ft_strlcpy(new_node->file_info.name.buff, ".", 257);
	new_node->file_info.name_type = E_FILE;
	parse_arg_type(data, new_node);
	if (!parse_arg(data, new_node, 1))
		return (ast_pool_clear(&args_pool), 0);
	if (data->flags.d)
		g_fill_buff_char('\n');
	return (1);
}

static int parse_sorted_args_array(t_data *data, t_ast **array, int array_len, t_pool_ast *args_pool)
{
	for (int i = 0; array[i]; i++)
	{
		if (!parse_arg(data, array[i], array_len))
			return (ast_pool_clear(&args_pool), 0);
		if (array[i]->file_info.error)
			continue ;
		if (array[i]->file_info.type != TYPE_DIR)
		{
			if (array[i + 1])
			{
				if (array[i + 1]->file_info.type != TYPE_DIR || data->flags.d)
					{ g_fill_buff_char(' '); g_fill_buff_char(' '); }
				else
					g_fill_buff_char('\n');
			}
			else
				g_fill_buff_char('\n');
		}
		else if (data->flags.d)
		{
			if (array[i + 1])
				{g_fill_buff_char(' '); g_fill_buff_char(' ');}
			else
				g_fill_buff_char('\n');
		}
	}
	return (1);
}

int parse_args_list(t_data *data, t_pool_ast *args_pool, int argc, char *argv[])
{
	int array_len = 0;
	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
			continue;
		t_ast *new_node = get_new_ast(&args_pool);
		if (!new_node)
			return (0);
		new_node->file_info.name.ptr = argv[i];
		new_node->file_info.name_type = E_PTR;
		parse_arg_type(data, new_node);
		array_len++;
	}
	t_ast **array = convert_to_array(args_pool);
	if (!array)
		return (ast_pool_clear(&args_pool), 0);
	sort_array_args(&array, data->flags);
	if (!parse_sorted_args_array(data, array, array_len, args_pool))
		return (free(array), 0);
	return (free(array), 1);
}
