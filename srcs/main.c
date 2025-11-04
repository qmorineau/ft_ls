#include "ft_ls.h"

void parse_arg(t_data *data, t_ast *new_node)
{
	push_path(data, &new_node->file_info);

	struct stat buff;
	if (lstat(data->path, &buff) == 0)
		new_node->file_info.type = stat_type_parser(&buff);
	else
		stat_error(data->path);
	// printf("path_len = %ld\n", data->path_len);
	// printf("parse arg path = %s\n", data->path);
	if (new_node->file_info.type == TYPE_DIR)
		parse_folder(data, &new_node->file_info);
	else
		parse_file_infos(data, &new_node);
}

t_data *parsing(int argc, char *argv[], char *envp[])
{
	t_data *data;
	t_pool_ast *args_pool = NULL;

	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		return (NULL);
	int count_option = option_parser(argc, argv, &data->flags);
	if (count_option == -1)
		return (free(data), NULL);
	data->now = time(NULL);
	parse_terminal(&data->term);
	if (data->term.is_tty && 0)
		parse_colors(data, envp);
	if (argc - count_option - 1 == 0)
	{
		t_ast *new_node = get_new_ast(&args_pool);
		ft_strlcpy(new_node->file_info.name.buff, ".", 257);
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
			// ast_addback(&data->tree, new_node);
		}
		// print(data, data->tree);
	}
	// printf("coucou\n");
	pool_clear(&args_pool);
	return data;
}

int main(int argc, char *argv[], char *envp[])
{
	t_data *data;

	data = parsing(argc, argv, envp);
	if (!data)
		return (2);
	test_flush();
	free_all_and_exit(&data, data->exit_status);
	return (0);
}