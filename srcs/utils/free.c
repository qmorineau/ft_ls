#include "ft_ls.h"

void free_all_and_exit(t_data *data, int exit_code)
{
	map_pool_clear(&data->colors);
	map_pool_clear(&data->file_colors);
	map_pool_clear(&data->user_id);
	map_pool_clear(&data->group_id);
	exit(exit_code);
}
