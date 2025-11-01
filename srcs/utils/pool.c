#include "ft_ls.h"

t_ast* get_new_ast(t_pool_ast **head)
{
	if (!*head)
	{
		t_pool_ast	*new_pool = ft_calloc(1, sizeof(t_pool_ast));
		if (!new_pool)
			exit(2); // manage error
		*head = new_pool;
		new_pool->pool = ft_calloc(POOL_ITEMS_NUMBER, sizeof(t_ast));
		if (!new_pool->pool)
			exit(2);
	}

	t_pool_ast *tmp = *head;
	while (tmp->next)
		tmp = tmp->next;

	if (tmp->it == POOL_ITEMS_NUMBER)
	{
		t_pool_ast	*new_pool = ft_calloc(1, sizeof(t_pool_ast));
		if (!new_pool)
			exit(2); // manage error
		tmp->next = new_pool;
		tmp = tmp->next;
		new_pool->pool = ft_calloc(POOL_ITEMS_NUMBER, sizeof(t_ast));
		if (!new_pool->pool)
			exit(2);
	}
	return (&tmp->pool[tmp->it++]);
}

void	pool_clear(t_pool_ast **head)
{
	t_pool_ast *to_free = *head;
	t_pool_ast *tmp;

	while (to_free)
	{
		tmp = to_free->next;
		free(to_free->pool);
		free(to_free);
		to_free = tmp;
	}
}
