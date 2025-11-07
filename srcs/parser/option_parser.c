#include "ft_ls.h"

static void override_options(t_flags *flags)
{
	if (flags->f)
	{
		flags->a = 1;
		flags->t = 0;
		flags->r = 0;
		flags->l = 0;
	}
	if (flags->d)
		flags->R = 0;
}

int option_parser(int argc, char *argv[], t_flags *flags)
{
	unsigned int count = 0;

	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-' && ft_strlen(argv[i]) > 1)
		{
			count++;
			for (size_t j = 1; j < ft_strlen(argv[i]); j++)
			{
				switch (argv[i][j])
				{
					case 'l':
						flags->l = 1;
						break;
					case 'R':
						flags->R = 1;
						break;
					case 'a':
						flags->a = 1;
						break;
					case 'r':
						flags->r = 1;
						break;
					case 't':
						flags->t = 1;
						break;
					case 'u':
						flags->u = 1;
						break;
					case 'f':
						flags->f = 1;
						break;
					case 'g':
						flags->g = 1;
						break;
					case 'd':
						flags->d = 1;
						break;
					default:
						fill_buff_error("ft_ls: invalid option -- '", 26);
						fill_buff_error(&argv[i][j], 1);
						fill_buff_error("'\n", 2);
						flush_error();
						return (-1);
				}
			}
		}
	}
	override_options(flags);
	return (count);
}
