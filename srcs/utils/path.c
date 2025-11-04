#include "ft_ls.h"

void pop_path(t_data *data)
{
	while (data->path_len && data->path[data->path_len] != '/')
		data->path_len--;
	data->path[data->path_len] = '\0';
}

void push_path(t_data *data, t_file *folder)
{
	size_t i = 0;
	char *name = get_name(folder);
	while (name[i])
	{
		if (data->path_len + i > PATH_MAX)
		{
			folder->error = ENAMETOOLONG;
			break;
		}
		data->path[data->path_len++ + i] = name[i];
		i++;
	}
	data->path[data->path_len++ + i] = 0;
}