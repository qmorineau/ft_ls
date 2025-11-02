#include "ft_ls.h"

t_ast* get_new_ast(t_pool_ast **head)
{
	if (!*head)
	{
		t_pool_ast	*new_pool = ft_calloc(1, sizeof(t_pool_ast));
		if (!new_pool)
			exit(2); // manage error
		*head = new_pool;
		return (&new_pool->pool[new_pool->it++]);
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
		free(to_free);
		to_free = tmp;
	}
}
