#include "ft_ls.h"

int match_file_patern(char **ext)
{
	if (ft_strlen(*ext) < 2)
		return (0);
	if (ext[0][0] == '*')
		return (1);
	else
		return (0);
}

ssize_t get_index(char *str, char c)
{
	ssize_t i = -1;
	while (str[++i])
	{
		if (str[i] == c)
		{
			if (i == 0)
				return (-1);
			return (i);
		}
	}
	return (-1);
}