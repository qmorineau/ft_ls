#include "ft_ls.h"

t_ast *new_ast_node()
{
	t_ast *node;

	node = ft_calloc(1, sizeof(t_ast));
	if (!node)
		return (NULL);
	return (node);
}

void ast_addback(t_ast **head, t_ast *new)
{
	if (!head || !*head)
	{
		*head = new;
		new->index = 0;
	}
	else
	{
		if (!(*head)->tail)
		{
			(*head)->next = new;
			(*head)->tail = new;
			new->index = 1;
		}
		else
		{
			new->index = (*head)->tail->index + 1;
			(*head)->tail->next = new;
			(*head)->tail = new;
		}
	}
}

void free_file_info(t_file *file)
{
	free(file->name);
	free(file->group_name);
	free(file->user_name);
	free(file->mod_time);
	free(file->access_time);
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