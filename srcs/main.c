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
	// check res
	if (argc - count_option - 1 == 0)
	{
		t_ast *new_node = new_ast_node();
		// check res
		new_node->path = ft_strdup(".");
		// check res
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
			parse_file_infos(&new_node, data->flags);
			ast_addback(&data->tree, new_node);
			parse_ast_node(&new_node, data->flags);
			// check res
		}
	}
	return data;
}

int main(int argc, char *argv[])
{
	t_data *data;

	data = parsing(argc, argv);
	// check data
	print(data);
	free_all(&data);
	return (0);
}