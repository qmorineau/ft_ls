#include "ft_ls.h"

void pop_path(t_data *data)
{
	if (!data->path_len)
		return; 

	while (data->path_len > 0 && data->path[data->path_len - 1] == '/')
		data->path_len--;
	if (!data->path_len)
	{
		data->path[0] = '/';
		data->path[1] = 0;
		data->path_len = 1;
	}
	else
		data->path[data->path_len] = 0;

	while (data->path_len > 0 && data->path[data->path_len] != '/')
		data->path_len--;
	if (data->path_len == 0)
	{
		data->path[0] = '/';
		data->path[1] = 0;
		data->path_len = 1;
	}
	else
		data->path[data->path_len] = '\0';
	
}

void push_path(t_data *data, t_file *folder)
{
	char *name = get_name(folder);
	size_t name_len = ft_strlen(name);

	if (data->path_len + name_len + 1 >= PATH_MAX)
	{
		folder->error = ENAMETOOLONG;
		return;
	}
	while (data->path_len > 0 && (data->path[data->path_len - 1] == '/' && data->path_len != 1))
		data->path_len--;
	if (data->path_len > 0 && data->path[data->path_len - 1] != '/')
		data->path[data->path_len++] = '/';
	ft_strlcpy(data->path + data->path_len, name, PATH_MAX - data->path_len);
	data->path_len += name_len;
}