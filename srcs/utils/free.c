#include "ft_ls.h"

static void free_map(t_map **map)
{
	if (!map || !*map)
		return ;
	t_map *tmp = *map;
	t_map *tmp2;

	while (tmp)
	{
		tmp2 = tmp->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
		tmp = tmp2;
	}
}

static void free_map_uid(t_map_uid **map)
{
	if (!map || !*map)
		return ;
	t_map_uid *tmp = *map;
	t_map_uid *tmp2;

	while (tmp)
	{
		tmp2 = tmp->next;
		free(tmp->value);
		free(tmp);
		tmp = tmp2;
	}
}

void free_all_and_exit(t_data **data, int exit_code)
{
	free_map(&(*data)->colors);	
	free_map(&(*data)->file_colors);
	free_map_uid(&(*data)->user_id);
	free_map_uid(&(*data)->group_id);
	free(*data);
	exit(exit_code);
}

void free_file_info(t_file *file)
{
	if (file->redirect_file)
		free(file->redirect_file);
}
