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

static void free_parse_colors(t_data *data, char *key, char *value, int is_exit)
{
	if (key)
		free(key);
	if (value)
		free(value);
	if (is_exit)
	{
		free_all(&data);
		exit(2);
	}
}

static ssize_t get_index(char *str, char c)
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

static void parse_default_color(t_data *data)
{
	for (int i = 0; key_value[i][0]; i++)
	{
		char *key = ft_strdup(key_value[i][0]);
		if (!key)
			free_parse_colors(data, key, NULL, 1);
		char *value = ft_strdup(key_value[i][1]);
		if (!value)
			free_parse_colors(data, key, value, 1);
		int res = map_set(&data->colors, &key, &value);
		if (!res)
			free_parse_colors(data, key, value, 1);
	}
}

static int match_file_patern(char **ext)
{
	if (ft_strlen(*ext) < 2)
		return (0);
	if (ext[0][0] == '*')
	{
		ft_memmove(&ext[0][0], &ext[0][1], ft_strlen(&ext[0][1]) + 1);
		return (1);
	}
	else
		return (0);
}

int index_ls_colors(char *envp[])
{
	for (int i = 0; envp[i]; i++)
	{
		if (strncmp("LS_COLORS=", envp[i], 10) == 0)
			return (i);
	}
	return (-1);
}

void	parse_colors(t_data *data, char *envp[])
{
	parse_default_color(data);

	int ls_colors_idx = index_ls_colors(envp);
	if (ls_colors_idx == -1)
		return ;
	char **array = ft_split(envp[ls_colors_idx], ':');
	if (!array)
		free_parse_colors(data, NULL, NULL, 1);
	// error

	memmove(&array[0][0], &array[0][10], ft_strlen(&array[0][10]) + 1);
	for (int i = 0; array[i]; i++)
	{
		ssize_t idx = get_index(array[i], '=');
		if (!array[i][0])
		{
			free(array[i]);
			continue ;
		}
		else if (idx == -1)
		{
			while (array[i])
				free(array[i++]);
			free(array);
			data->color_parse_error = 1;
			return ;
		}
		char *key = NULL;
		char *value = NULL;
		key = ft_strndup(array[i], idx);
		if (!key)
			free_parse_colors(data, NULL, NULL, 1);
		value = ft_strdup(&array[i][idx + 1]);
		if (!value)
			free_parse_colors(data, key, NULL, 1);
		if (!key[2] && key[0] != '*')
		{
			if (map_get(data->colors, key))
				map_set(&data->colors, &key, &value);
			else
			{
				ft_printf("ft_ls: unrecognize prefix: '%s'\n", key);
				data->color_parse_error = 1;
				free_parse_colors(data, key, value, 0);
				while (array[i])
					free(array[i++]);
				free(array);
				return ;
			}
		}
		else
		{
			if (match_file_patern(&key))
				map_set(&data->file_colors, &key, &value);
			else
			{
				free_parse_colors(data, key, value, 0);
				data->color_parse_error = 1;
			}
		}
		free(array[i]);
	}
	free(array);
}
