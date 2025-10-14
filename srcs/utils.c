#include "ft_ls.h"

void free_parent_ast(t_data **data)
{
	if (!data || !*data)
		return ;
	for (int i = 0; (*data)->args[i]; i++)
	{
		free((*data)->args[i]->path);
		ast_clear(&(*data)->args[i]);
		free((*data)->args[i]);
	}
	free((*data)->args);
}

void free_all(t_data **data)
{
	free_parent_ast(data);
	free(*data);
}