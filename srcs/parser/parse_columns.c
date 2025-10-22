#include "ft_ls.h"

static size_t parse_user_max_length(t_ast *head)
{
	size_t max_len = 0;

	t_ast *tmp_node = head;
	while (tmp_node)
	{
		size_t tmp = ft_strlen(tmp_node->file_info.user_name);
		if (tmp > max_len)
			max_len = tmp;
		tmp_node = tmp_node->next;
	}
	return (max_len);
}

static size_t parse_group_max_length(t_ast *head)
{
	size_t max_len = 0;

	t_ast *tmp_node = head;
	while (tmp_node)
	{
		size_t tmp = ft_strlen(tmp_node->file_info.group_name);
		if (tmp > max_len)
			max_len = tmp;
		tmp_node = tmp_node->next;
	}
	return (max_len);
}

static size_t parse_size_max_length(t_ast *head)
{
	size_t max_len = 0;

	t_ast *tmp_node = head;
	while (tmp_node)
	{
		size_t count = 0;
		size_t tmp = tmp_node->file_info.size;
		while (tmp >= 10)
		{
			tmp /= 10;
			count++;
		}
		count++;
		if (count > max_len)
			max_len = count;
		tmp_node = tmp_node->next;
	}
	return (max_len);
}

t_columns	*parse_columns(t_ast *node)
{
	t_columns *data = ft_calloc(1, sizeof(t_columns));

	if (!data)
		return (NULL);

	data->user_max_len = parse_user_max_length(node->head);
	data->group_max_len = parse_group_max_length(node->head);
	data->size_max_len = parse_size_max_length(node->head);

	return (data);
}