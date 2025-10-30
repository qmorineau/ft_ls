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

void free_all_and_exit(t_data **data, int exit_code)
{
	ast_clear(&(*data)->tree);
	free_map(&(*data)->colors);	
	free_map(&(*data)->file_colors);
	free(*data);
	exit(exit_code);
}

void free_file_info(t_file *file)
{
	free(file->group_name);
	free(file->user_name);
	if (file->name_type == PTR)
		free(file->name.ptr);
	if (file->redirect_file)
	{
		if (file->redirect_file->name_type == PTR)
			free(file->redirect_file->name.ptr);
		free(file->redirect_file);
	}
}
