#include "ft_ls.h"

t_ast **convert_to_array(t_pool_ast *pool)
{
	t_pool_ast *tmp = pool;
	unsigned int length = 0;
	while (tmp)
	{
		length += tmp->it;
		tmp = tmp->next;
	}

	t_ast **array = malloc(length * sizeof(t_ast *));
	if (!array)
		return (NULL);

	unsigned int it = 0;
	while (pool)
	{
		int i = 0;
		while (i < pool->it)
			array[it++] = &pool->pool[i++];
		pool = pool->next;
	}
	array[length] = NULL;
	return (array);
}
