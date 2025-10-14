#include "ft_ls.h"

void print(t_flags *flags)
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

void print_data(t_data *data)
{
	for (int i = 0; data->args[i]; i++)
	{
		ft_printf("args: path = %s, type = %d\n", data->args[i]->path, data->args[i]->type);
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
	(void) argv[argc];
	t_data *data;

	if (argc == 1)
	{
		data = parsing(argc, argv);
		print_data(data);
		print(&data->flags);
	}
	else
	{
		data = parsing(argc, argv);
		print_data(data);
		print(&data->flags);


		// DIR* dir = opendir(argv[1]);

		// struct dirent *tmp = readdir(dir);
		// while (tmp)
		// {
		// 	printf("file = %s\n", tmp->d_name);
		// 	tmp = readdir(dir);
		// }
		// closedir(dir);
		// print(&data);


		//parsing
		/* 
			check flag, multiple flag, overide flag when needed etc...

		*/

		// ft_ls(/* idk */, /* res of parsing */);

	}
	free_all(&data);
	return (0);
}