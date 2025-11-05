#include "ft_ls.h"

void parse_arg(t_data *data, t_ast *new_node)
{
	push_path(data, &new_node->file_info);

	if (lstat(data->path, &new_node->file_info.sb) == 0)
		new_node->file_info.type = stat_type_parser(&new_node->file_info.sb);
	else
		stat_error(data->path);

	if (new_node->file_info.type == TYPE_DIR)
		parse_folder(data, &new_node->file_info);
	else
		parse_file_infos(data, &new_node);
}

int	parsing(t_data *data, int argc, char *argv[], char *envp[])
{
	t_pool_ast *args_pool = NULL;

	int count_option = option_parser(argc, argv, &data->flags);
	if (count_option == -1)
		return (0);
	data->now = time(NULL);
	parse_terminal(&data->term);
	if (data->term.is_tty)
		parse_colors(data, envp);
	if (argc - count_option - 1 == 0)
	{
		t_ast *new_node = get_new_ast(&args_pool);
		ft_strlcpy(new_node->file_info.name.buff, ".", 257);
		new_node->file_info.name_type = E_FILE;
		parse_arg(data, new_node);
	}
	else
	{

		for (int i = 1; i < argc; i++)
		{
			if (argv[i][0] == '-')
				continue;
			t_ast *new_node = get_new_ast(&args_pool);
			ft_strlcpy(new_node->file_info.name.path, argv[i], PATH_MAX);
			new_node->file_info.name_type = E_PATH;

			parse_arg(data, new_node);
		}
	}
	pool_clear(&args_pool);
	return (1);
}

int main(int argc, char *argv[], char *envp[])
{
	t_data data = {0};

	ft_memset(&data, 0, sizeof(t_data));
	if (!parsing(&data, argc, argv, envp))
	{
		//manage error
	}
	test_flush();
	free_all_and_exit(&data, data.exit_status);
	return (0);
}