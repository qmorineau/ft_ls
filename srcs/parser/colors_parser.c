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
		free_all_and_exit(data, data->exit_status);
}

static void parse_default_color(t_data *data)
{
	for (int i = 0; key_value[i][0]; i++)
	{
		char *key = ft_strdup(key_value[i][0]);
		char *value = ft_strdup(key_value[i][1]);
		if (!key || !value || !map_set(&data->colors, &key, &value))
		{
			data->exit_status = 2;
			free_parse_colors(data, key, value, 1);
		}
	}
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
	{
		data->exit_status = 2;
		free_parse_colors(data, NULL, NULL, 1);
	}

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
		value = ft_strdup(&array[i][idx + 1]);
		if (!key || !value)
		{
			data->exit_status = 2;
			free_parse_colors(data, NULL, NULL, 1);
		}
		if (!key[2] && key[0] != '*')
		{
			if (map_get(data->colors, key))
				map_set(&data->colors, &key, &value);
			else
			{
				ft_putstr_fd("ft_ls: unrecognize prefix: '", 2);
				ft_putstr_fd(key, 2);
				ft_putstr_fd("'\n", 2);
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

t_map *get_colors(t_map *file_colors, t_map *colors, t_file *file)
{
	switch (file->type)
	{
		case TYPE_FILE:
			if (ft_strchr(file->permissions, 'S'))
			{
				if (file->permissions[3] == 'S')
					return (map_get(colors, "su"));
				else
					return (map_get(colors, "sg"));
			}
			else if (ft_strchr(file->permissions, 'x'))
				return (map_get(colors, "ex"));
			else 
			{
				t_map *tmp = find_extension(file_colors, get_name(file));
				if (!tmp)
					return (map_get(colors, "fi"));
				return (tmp);
			}
		case TYPE_DIR:
			if (ft_strchr(file->permissions, 't'))
				return (map_get(colors, "ow"));
			else
				return (map_get(colors, "di"));
		case TYPE_LINK:
			if (file->redirect_file->type == TYPE_BROKEN_LINK)
				return (map_get(colors, "or"));
			else
				return (map_get(colors, "ln"));
		case TYPE_BROKEN_LINK:
			return (map_get(colors, "or"));
		case TYPE_BLOCK:
			return (map_get(colors, "bd"));
		case TYPE_PIPE:
			return (map_get(colors, "pi"));
		case TYPE_SOCKET:
			return (map_get(colors, "so"));
		case TYPE_CHR:
			return (map_get(colors, "cd"));
		default:
			return (NULL);
	}
}
