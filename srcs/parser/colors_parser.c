#include "ft_ls.h"

static const char *key_value[][2] =
{
	{"rs", "0"},
	{"di", "01;34"},
	{"ln", "01;36"},
	{"mh", "00"},
	{"pi", "40;33"},
	{"so", "01;35" },
	{"do", "01;35" },
	{"bd", "40;33;01"},
	{"cd", "40;33;01"},
	{"or", "40;31;01"},
	{"mi", "00"},
	{"su", "37;41"},
	{"sg", "30;43"},
	{"ca", "30;41"},
	{"tw", "30;42"},
	{"ow", "34;42"},
	{"st", "37;44"},
	{"ex", "01;32"},
	{NULL, NULL}
};

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

static int parse_default_color(t_data *data)
{
	for (int i = 0; key_value[i][0]; i++)
	{
		char *key = ft_strdup(key_value[i][0]);
		if (!key)
			return (0);
		char *value = ft_strdup(key_value[i][1]);
		if (!value)
		{
			free(key);
			return (0);
		}
		int res = map_set(&data->colors, &key, &value);
		if (!res)
		{
			free(key);
			free(value);
			return (0);
		}
	}
	return (1);
}

char **search_env(char *envp[])
{
	char **array = NULL;

	for (int i = 0; envp[i]; i++)
	{
		if (strncmp("LS_COLORS=", envp[i], 10) == 0)
		{
			array = ft_split(envp[i], ':');
			break;
		}
	}
	return array;
}

void	parse_colors(t_data *data, char *envp[])
{
	if (!parse_default_color(data))
	{
		free_all(&data);
		exit(2);
	}

	char **array = search_env(envp);
	if (!array)
		exit(2); // manage error

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
		if (!key[2])
		{
			if (map_get(data->colors, key))
				map_set(&data->colors, &key, &value);
			else
			{
				data->color_parse_error = 1;
				ft_printf("ft_ls: unrecognize prefix: '%s'\n", key);
				free(key);
				free(value);
				while (array[i])
					free(array[i++]);
				free(array);
				return ;
			}
		}
		else
			map_set(&data->file_colors, &key, &value);
		free(array[i]);
	}
	free(array);
}