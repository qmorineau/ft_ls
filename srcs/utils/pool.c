#include "ft_ls.h"

static void reset_file(t_file *file)
{
	file->error = 0;
	file->redirect_file = NULL;
	file->acl_char = ' ';
}

t_ast* get_new_ast(t_pool_ast **head)
{
	if (!*head)
	{
		t_pool_ast	*new_pool = malloc(sizeof(t_pool_ast));
		if (!new_pool)
			exit(2); // manage error
		new_pool->it = 0;
		new_pool->next = NULL;
		*head = new_pool;
		reset_file(&new_pool->pool[new_pool->it].file_info);
		return (&new_pool->pool[new_pool->it++]);
	}

	t_pool_ast *tmp = *head;
	while (tmp->next)
		tmp = tmp->next;

	if (tmp->it == POOL_ITEMS_NUMBER)
	{
		t_pool_ast	*new_pool = malloc(sizeof(t_pool_ast));
		if (!new_pool)
			exit(2); // manage error
		new_pool->it = 0;
		new_pool->next = NULL;
		tmp->next = new_pool;
		tmp = tmp->next;
	}
	reset_file(&tmp->pool[tmp->it].file_info);
	return (&tmp->pool[tmp->it++]);
}

void	pool_clear(t_pool_ast **pool_head)
{
	t_pool_ast *to_free = *pool_head;
	t_pool_ast *tmp;

	while (to_free)
	{
		for (int i = 0; i < to_free->it; i++)
			ast_clear(&to_free->pool[i]);
		tmp = to_free->next;
		free(to_free);
		to_free = tmp;
	}
}
