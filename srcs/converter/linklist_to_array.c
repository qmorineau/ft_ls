#include "ft_ls.h"

t_ast **convert_to_array(t_ast *head)
{
	const int length = ast_length(head);

	t_ast **array = ft_calloc(length + 1, sizeof(t_ast *));
	if (!array)
		return (NULL);
	t_ast *tmp = head;
	int i = 0;
	while (tmp)
	{
		array[i++] = tmp;
		tmp = tmp->next;
	}
	return (array);
}