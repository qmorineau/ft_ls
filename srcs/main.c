#include "ft_ls.h"

void print_flags(t_flags *flags)
{
	ft_printf("Print: \n");
	ft_printf("option_l = %d\n", flags->l);
	ft_printf("option_R = %d\n", flags->R);
	ft_printf("option_a = %d\n", flags->a);
	ft_printf("option_r = %d\n", flags->r);
	ft_printf("option_t = %d\n", flags->t);
	ft_printf("option_u = %d\n", flags->u);
	ft_printf("option_f = %d\n", flags->f);
	ft_printf("option_g = %d\n", flags->g);
	ft_printf("option_d = %d\n", flags->d);
}

void error(char *error)
{
	write(2, error, ft_strlen(error));
	exit(2);
}

t_data *parsing(int argc, char *argv[])
{
	t_data *data;

	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		return (NULL);
	int count_option = option_parser(argc, argv, &data->flags);
	parse_terminal(&data->term);
	// check res
	if (argc - count_option - 1 == 0)
	{
		t_ast *new_node = new_ast_node();
		// check res
		new_node->path = ft_strdup(".");
		// check res
		struct stat buff;
		if (stat(new_node->path, &buff) == 0)
			new_node->file_info.type = stat_type_parser(&buff);
		else
			exit(1); // error
		parse_file_infos(&new_node, data->flags);
		parse_ast_node(&new_node, data->flags);
		ast_addback(&data->tree, new_node);
	}
	else
	{
		for (int i = 1; i < argc; i++)
		{
			if (argv[i][0] == '-')
				continue;
			t_ast *new_node = new_ast_node();
			// check res
			new_node->path = argv[i][strlen(argv[i]) - 1] == '/' ? ft_strndup(argv[i], strlen(argv[1]) - 1) : ft_strdup(argv[i]);
			// check res
			struct stat buff;
			if (stat(new_node->path, &buff) == 0)
				new_node->file_info.type = stat_type_parser(&buff);
			else
				exit(1); // error
			parse_file_infos(&new_node, data->flags);
			ast_addback(&data->tree, new_node);
			parse_ast_node(&new_node, data->flags);
			// check res
		}
	}
	return data;
}

void test(char *envp[])
{
	char **array;
	for (int i = 0; envp[i]; i++)
	{
		if (strncmp("LS_COLORS=", envp[i], 10) == 0)
			array = ft_split(envp[i], ':');
	}
	if (!array)
		return ; //error
	memmove(&array[0][0], &array[0][10], strlen(&array[0][10]) + 1);
	for (int i = 0; array[i]; i++)
	{
		printf("elem %d = %s\n", i, array[i]);
		free(array[i]);
	}
	free(array);
}

int main(int argc, char *argv[], char *envp[])
{
	t_data *data;

	data = parsing(argc, argv);
	// check data
	// test(envp);
	(void) envp;
	print(data);
	free_all(&data);
	return (0);
}