#include "ft_ls.h"

static size_t parse_len_size_t(size_t nbr)
{
	size_t count = 0;
	while (nbr >= 10)
	{
		nbr /= 10;
		count++;
	}
	count++;
	return (count);
}

static size_t get_largest_len(size_t max_len, char *str)
{
	size_t tmp = ft_strlen(str);
	if (tmp > max_len)
		max_len = tmp;
	return max_len;
}

static size_t parse_user_max_length(t_ast *head)
{
	size_t max_len = 0;

	t_ast *tmp_node = head;
	while (tmp_node)
	{
		max_len = get_largest_len(max_len, tmp_node->file_info.user_name);
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
		max_len = get_largest_len(max_len, tmp_node->file_info.group_name);
		tmp_node = tmp_node->next;
	}
	return (max_len);
}

static size_t parse_size_max_length(t_ast *head)
{
	size_t count;
	size_t max_len = 0;

	t_ast *tmp_node = head;
	while (tmp_node)
	{
		if (tmp_node->file_info.type == TYPE_CHR || tmp_node->file_info.type == TYPE_BLOCK)
			count = parse_len_size_t(tmp_node->file_info.minor) + parse_len_size_t(tmp_node->file_info.major) + 2;
		else
			count = parse_len_size_t(tmp_node->file_info.size);
		if (count > max_len)
			max_len = count;
		tmp_node = tmp_node->next;
	}
	return (max_len);
}

static size_t parse_minor_max_length(t_ast *head)
{
	size_t count;
	size_t max_len = 0;

	t_ast *tmp_node = head;
	while (tmp_node)
	{
		if (tmp_node->file_info.type == TYPE_CHR || tmp_node->file_info.type == TYPE_BLOCK)
			count = parse_len_size_t(tmp_node->file_info.minor);
		else
		{
			tmp_node = tmp_node->next;
			continue;
		}
		if (count > max_len)
			max_len = count;
		tmp_node = tmp_node->next;
	}
	return (max_len);
}

static size_t parse_major_max_length(t_ast *head)
{
	size_t count;
	size_t max_len = 0;

	t_ast *tmp_node = head;
	while (tmp_node)
	{
		if (tmp_node->file_info.type == TYPE_CHR || tmp_node->file_info.type == TYPE_BLOCK)
			count = parse_len_size_t(tmp_node->file_info.major);
		else
		{
			tmp_node = tmp_node->next;
			continue;
		}
		if (count > max_len)
			max_len = count;
		tmp_node = tmp_node->next;
	}
	return (max_len);
}

static size_t parse_link_max_length(t_ast *head)
{
	size_t max_len = 0;

	t_ast *tmp_node = head;
	while (tmp_node)
	{
		size_t count = parse_len_size_t(tmp_node->file_info.link);
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
	data->minor_max_len = parse_minor_max_length(node->head);
	data->major_max_len = parse_major_max_length(node->head);
	data->link_max_len = parse_link_max_length(node->head);

	return (data);
}