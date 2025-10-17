#include "ft_ls.h"

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
	free(*data);
}