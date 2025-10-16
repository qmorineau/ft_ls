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

void print_file(t_file *file)
{
	printf("name = %s\n", file->name);
	printf("size = %zd\n", file->size);
	printf("perm = %o\n", file->permissions); // need to convert from decimal to octal to get perm
}

void print_ast(t_ast *node)
{
	printf("path = %s\n", node->path);
	printf("type = %d\n\n", node->file_info.type);
	print_file(&node->file_info);

	t_ast *tmp = node->head;
	while (tmp)
	{
		print_ast(tmp);
		tmp = tmp->next;
	}
}

void print_data(t_data *data)
{
	for (int i = 0; data->args[i]; i++)
	{
		print_ast(data->args[i]);
	}
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
	int count_option = option_parser(argc, argv, &data->flags); // check res
	data->args = ft_calloc((argc - count_option), sizeof(t_ast *));
	if (!data->args)
		return (NULL);
	parse_arguments(argc, argv, data); // check res
	parse_data(data); // check res

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