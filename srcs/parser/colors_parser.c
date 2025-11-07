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
		if (!key || !value || !map_set(&data->colors, key, value, STR))
		{
			data->exit_status = 2;
			free_parse_colors(data, key, value, 1);
		}
	}
}

static int index_ls_colors(char *envp[])
{
	for (int i = 0; envp[i]; i++)
	{
		if (ft_strncmp("LS_COLORS=", envp[i], 10) == 0)
			return (i);
	}
	return (-1);
}

static int parse_special_files_colors(t_data *data, char *key, char *value)
{
	if (map_get(data->colors, key))
	{
		if (!map_set(&data->colors, key, value, STR))
		{
			data->exit_status = 2;
			free_parse_colors(data, NULL, NULL, 1);
		}
		return (1);
	}
	else
	{
		fill_buff_error("ft_ls: unrecognize prefix: '", 28);
		fill_buff_error(key, ft_strlen(key));
		fill_buff_error("'\n", 2);
		flush_error();
		data->color_parse_error = 1;
		free_parse_colors(data, key, value, 0);
		return (0);	
	}
}

static void parse_file_extension_colors(t_data *data, char *key, char *value)
{
	if (match_file_patern(&key))
	{
		if (!map_set(&data->file_colors,key, value, STR))
		{
			data->exit_status = 2;
			free_parse_colors(data, NULL, NULL, 1);
		}
	}
	else
	{
		free_parse_colors(data, key, value, 0);
		data->color_parse_error = 1;
	}
}

static int is_valid_color(t_data *data, char **array, ssize_t idx, int i)
{
	if (!array[i][0])
	{
		free(array[i]);
		return (0);
	}
	else if (idx == -1)
	{
		while (array[i])
			free(array[i++]);
		data->color_parse_error = 1;
		return (-1);
	}
	return (1);
}

static int parse_colors_loop(t_data *data, char **array, int i)
{
	ssize_t idx = get_index(array[i], '=');
	int res = is_valid_color(data, array, idx, i);
	if (res == -1)
		return (0);
	else if (res == 0)
		return (1);
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
		if (!parse_special_files_colors(data, key, value))
		{
			while (array[i])
				free(array[i++]);
			return (0);
		}
	}
	else
		parse_file_extension_colors(data, key, value);
	free(array[i]);
	return (1);
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

	ft_memmove(&array[0][0], &array[0][10], ft_strlen(&array[0][10]) + 1);
	for (int i = 0; array[i]; i++)
	{
		if (!parse_colors_loop(data, array, i))
			break;
	}
	free(array);
}

t_map *get_colors(t_map_pool *file_colors, t_map_pool *colors, t_file *file)
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
			if (file->redirect_file && file->redirect_file->type == TYPE_BROKEN_LINK)
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
