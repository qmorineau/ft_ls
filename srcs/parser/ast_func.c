#include "ft_ls.h"

t_ast *new_ast_node()
{
	t_ast *node;

	node = ft_calloc(1, sizeof(t_ast));
	if (!node)
		return (NULL);
	return (node);
}

void ast_addfront(t_ast **head, t_ast *new)
{
	if (!head || !*head)
		*head = new;
	else
	{
		new->next = *head;
		*head = new;
	}
}

void free_file_info(t_file *file)
{
	free(file->name);
}

void ast_clear(t_ast **node)
{
	t_ast *tmp;
	t_ast *tmp2;

	if (node && *node)
	{
		tmp = (*node);
		while (tmp)
		{
			tmp2 = tmp->next;
			free_file_info(&tmp->file_info);
			free(tmp->path);
			if (tmp->head)
				ast_clear(&tmp->head);
			free(tmp);
			tmp = tmp2;
		}
	}
}

unsigned int ast_length(t_ast *head)
{
	unsigned int count = 0;
	t_ast *tmp;

	tmp = head;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return count;
}