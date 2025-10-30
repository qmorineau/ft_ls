#include "ft_ls.h"

void parse_arg(t_data *data, t_ast *new_node)
{
	struct stat buff;
	if (stat(new_node->path, &buff) == 0)
		new_node->file_info.type = stat_type_parser(&buff);
	else
		exit(1); // error
	if (new_node->file_info.type != TYPE_DIR)
	{
		char *tmp = strrchr(new_node->path, '/');
		if (tmp)
			strcpy(new_node->file_info.name, ++tmp);
		else
			strcpy(new_node->file_info.name, new_node->path);
	}
	parse_file_infos(data, &new_node);
	parse_ast_node(data, &new_node);
	ast_addback(&data->tree, new_node);
}

t_data *parsing(int argc, char *argv[])
{
	t_data *data;

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
		t_ast *new_node = new_ast_node();
		// check res
		new_node->path = ft_strdup(".");
		//check res
		parse_arg(data, new_node);
	}
	else
	{
		for (int i = 1; i < argc; i++)
		{
			if (argv[i][0] == '-')
				continue;
			t_ast *new_node = new_ast_node();
			// check res
			new_node->path = argv[i][strlen(argv[i]) - 1] == '/' ? ft_strndup(argv[i], ft_strlen(argv[1]) - 1) : ft_strdup(argv[i]);
			// check res
			parse_arg(data, new_node);
			// check res
		}
	}
	return data;
}

int main(int argc, char *argv[], char *envp[])
{
	t_data *data;

	data = parsing(argc, argv);
	if (!data)
		return (2);
	parse_colors(data, envp);
	print(data);
	free_all(&data);
	return (0);
}