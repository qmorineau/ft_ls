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
	//default config
	for (int i = 0; key_value[i][0]; i++)
	{
		char *key = ft_strdup(key_value[i][0]);
		//CHECK RES
		char *value = ft_strdup(key_value[i][1]);
		// check res
		int res = map_set(&data->colors, &key, &value);
		(void) res;
		// check res
	}

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
	free(array);
}