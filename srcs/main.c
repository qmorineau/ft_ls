#include "ft_ls.h"

void parse_arg(t_data *data, t_ast *new_node)
{
	struct stat buff;
	if (lstat(new_node->path, &buff) == 0)
		new_node->file_info.type = stat_type_parser(&buff);
	else
	{
		stat_error(new_node->path);
		ast_clear(&new_node);
		return ;
	}
	new_node->file_info.name_type = PTR;
	new_node->file_info.name.ptr = new_node->path;
	ast_addback(&data->tree, new_node);
	parse_file_infos(data, &new_node);
	// parse_ast_node(data, &new_node);
}

t_data *parsing(int argc, char *argv[])
{
	t_data *data;
	t_pool_ast *args_pool = NULL;

	char path[PATH_MAX];

	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		return (NULL);
	int count_option = option_parser(argc, argv, &data->flags);
	if (count_option == -1)
		return (free(data), NULL);
	data->now = time(NULL);
	parse_terminal(&data->term);
	if (argc - count_option - 1 == 0)
	{
		t_ast *new_node = get_new_ast(&args_pool);
		ft_strlcpy(path, ".", PATH_MAX);
		new_node->path = path;
		parse_arg(data, new_node);
		print(data, data->tree);
	}
	else
	{
		for (int i = 1; i < argc; i++)
		{
			if (argv[i][0] == '-')
				continue;
			t_ast *new_node = get_new_ast(&args_pool);
			new_node->path = argv[i][strlen(argv[i]) - 1] == '/' ? ft_strndup(argv[i], ft_strlen(argv[1]) - 1) : ft_strdup(argv[i]);
			if (!new_node->path)
			{
				ast_clear(&new_node);
				free_all_and_exit(&data, 2);
			}
			parse_arg(data, new_node);
		}
		print(data, data->tree);
	}
	pool_clear(&args_pool);
	return data;
}

int main(int argc, char *argv[], char *envp[])
{
	t_data *data;

	data = parsing(argc, argv);
	if (!data)
		return (2);
	parse_colors(data, envp);
	// print(data);
	free_all_and_exit(&data, data->exit_status);
	return (0);
}