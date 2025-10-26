#include "ft_ls.h"

static ssize_t get_index(char *str, char c)
{
	ssize_t i = -1;
	while (str[++i])
	{
		if (str[i] == c)
			return (i);
	}
	return (-1);
}

void	parse_colors(t_data *data, char *envp[])
{
	char **array = NULL;
	for (int i = 0; envp[i]; i++)
	{
		if (strncmp("LS_COLORS=", envp[i], 10) == 0)
		{
			array = ft_split(envp[i], ':');
			if (!array)
				exit(1); //manage error
		}
	}
	if (!array)
		return ; //default color
	else
	{
		memmove(&array[0][0], &array[0][10], strlen(&array[0][10]) + 1);
		for (int i = 0; array[i]; i++)
		{
			ssize_t idx = get_index(array[i], '=');
			if (idx == -1 || !idx)
				continue;
			char *key = NULL;
			char *value = NULL;
			key = ft_strndup(array[i], idx);
			if (!key)
			{
				free_all(&data);
				exit(2);
			}
			value = ft_strdup(&array[i][idx + 1]);
			if (!value)
			{	
				free(key);
				free_all(&data);
				exit(2);
			}
			map_set(&data->colors, &key, &value);
			free(array[i]);
		}
	}
	free(array);
}