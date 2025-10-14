#include "ft_ls.h"

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
				switch (argv[i][j]) // put to zero when it overide another flag
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
						error("ft_ls: invalid");
						break;
				}
			}
		}
	}
	return (count);
}

