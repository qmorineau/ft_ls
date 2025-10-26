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

void free_all(t_data **data)
{
	t_ast *tmp = (*data)->tree;
	t_ast *tmp2;

	while (tmp)
	{
		tmp2 = tmp->next;
		ast_clear(&tmp);
		tmp = tmp2;
	}
	free_map(&(*data)->colors);
	free(*data);
}

void free_file_info(t_file *file)
{
	free(file->group_name);
	free(file->user_name);
	free(file->mod_time);
	free(file->access_time);
}
