#include "ft_ls.h"

// void parse_arg_type(t_data *data, t_ast *new_node)
// {
// 	push_path(data, &new_node->file_info);
// 	if (statx(AT_FDCWD, data->path, AT_STATX_SYNC_AS_STAT, data->stax_mask , &new_node->file_info.sb) == 0)
// 		new_node->file_info.type = stat_type_parser(&new_node->file_info.sb);
// 	else if (errno == EPERM || errno == EACCES)
// 	{
// 		if (statx(AT_FDCWD, data->path, AT_SYMLINK_NOFOLLOW, data->stax_mask , &new_node->file_info.sb) == 0)
// 			new_node->file_info.type = stat_type_parser(&new_node->file_info.sb);
// 		else
// 		{
// 			data->exit_status = 2;
// 			new_node->file_info.error = ENOENT;
// 		}
// 	}
// 	else
// 	{
// 		data->exit_status = 2;
// 		new_node->file_info.error = ENOENT;
// 	}
// 	pop_path(data);
// }

// int parse_arg(t_data *data, t_ast *new_node, int array_len)
// {
// 	push_path(data, &new_node->file_info);
// 	if (data->flags.d && !new_node->file_info.error)
// 		parse_file_from_stat(data, &new_node->file_info);
// 	if (new_node->file_info.type == TYPE_DIR)
// 	{
// 		if (array_len == 1 && !data->flags.R)
// 			{if (!parse_folder(data, &new_node->file_info, 0)) return (0);}
// 		else
// 			{if (!parse_folder(data, &new_node->file_info, 2)) return (0);}
// 	}
// 	else
// 	{
// 		if (!parse_file_infos(data, &new_node, AT_FDCWD))
// 			return (0);
// 		if (!new_node->file_info.error)
// 			print_file(new_node->file_info, data, NULL);
// 		else
// 			print_error(data, new_node->file_info);
// 		if (data->first_print)
// 			data->first_print = 0;
// 	}
// 	pop_path(data);
// 	return (1);
// }

unsigned int statx_mask_parser(t_flags flags)
{
	unsigned int mask = 0;

	if (flags.l || flags.g)
	{
		mask |= STATX_NLINK | STATX_GID | STATX_SIZE;
		if (!flags.d)
			mask |= STATX_BLOCKS;
		if (!flags.g)
			mask |= STATX_UID;
		if (flags.u)
			mask |= STATX_ATIME;
		else
			mask |= STATX_MTIME;
	}
	else if (flags.u)
		mask |= STATX_ATIME;
	else if (flags.t)
		mask |= STATX_MTIME;
	return (mask);
}

int	parsing(t_data *data, int argc, char *argv[], char *envp[])
{
	t_pool_ast *args_pool = NULL;

	int count_option = option_parser(argc, argv, &data->flags);
	if (count_option == -1)
		return (0);
	data->stax_mask = statx_mask_parser(data->flags);
	data->now = time(NULL);
	parse_terminal(&data->term);
	if (data->term.is_tty)
		parse_colors(data, envp);
	if (data->color_parse_error)
		ft_putstr_fd("ft_ls: unparsable value for LS_COLORS environment variable\n", 2);
	if (argc - count_option - 1 == 0)
		parse_root(data, args_pool);
	else
		parse_args_list(data, args_pool, argc, argv);
	ast_pool_clear(&args_pool);
	return (1);
}

int main(int argc, char *argv[], char *envp[])
{
	t_data data = {0};

	ft_memset(&data, 0, sizeof(t_data));
	data.first_print = 1;
	if (!parsing(&data, argc, argv, envp))
		return (2);
	g_flush();
	free_all_and_exit(&data, data.exit_status);
	return (0);
}