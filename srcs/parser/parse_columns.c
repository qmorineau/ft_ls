#include "ft_ls.h"

// static size_t parse_len_size_t(size_t nbr)
// {
// 	size_t count = 0;
// 	while (nbr >= 10)
// 	{
// 		nbr /= 10;
// 		count++;
// 	}
// 	count++;
// 	return (count);
// }

static size_t get_largest_len(size_t max_len, char *str)
{
	if (!str)
		return (0);
	size_t tmp = ft_strlen(str);
	if (tmp > max_len)
		max_len = tmp;
	return max_len;
}

static size_t parse_user_max_length(t_data *data, t_ast *head)
{
	size_t max_len = 0;

	t_ast *tmp_node = head;
	while (tmp_node)
	{
		max_len = get_largest_len(max_len, map_get_id(data->user_id, tmp_node->file_info.sb.st_uid)->value);
		tmp_node = tmp_node->next;
	}
	return (max_len);
}

static size_t parse_group_max_length(t_data *data, t_ast *head)
{
	size_t max_len = 0;

	t_ast *tmp_node = head;
	while (tmp_node)
	{
		max_len = get_largest_len(max_len, map_get_id(data->group_id, tmp_node->file_info.sb.st_gid)->value);
		tmp_node = tmp_node->next;
	}
	return (max_len);
}

// static size_t parse_size_max_length(t_ast *head)
// {
// 	size_t count;
// 	size_t max_len = 0;

// 	t_ast *tmp_node = head;
// 	while (tmp_node)
// 	{
// 		if (tmp_node->file_info.type == TYPE_CHR || tmp_node->file_info.type == TYPE_BLOCK)
// 			count = parse_len_size_t(tmp_node->file_info.minor) + parse_len_size_t(tmp_node->file_info.major) + 2;
// 		else
// 			count = parse_len_size_t(tmp_node->file_info.size);
// 		if (count > max_len)
// 			max_len = count;
// 		tmp_node = tmp_node->next;
// 	}
// 	return (max_len);
// }

// static size_t parse_minor_max_length(t_ast *head)
// {
// 	size_t count;
// 	size_t max_len = 0;

// 	t_ast *tmp_node = head;
// 	while (tmp_node)
// 	{
// 		if (tmp_node->file_info.type == TYPE_CHR || tmp_node->file_info.type == TYPE_BLOCK)
// 			count = parse_len_size_t(tmp_node->file_info.minor);
// 		else
// 		{
// 			tmp_node = tmp_node->next;
// 			continue;
// 		}
// 		if (count > max_len)
// 			max_len = count;
// 		tmp_node = tmp_node->next;
// 	}
// 	return (max_len);
// }

// static size_t parse_major_max_length(t_ast *head)
// {
// 	size_t count;
// 	size_t max_len = 0;

// 	t_ast *tmp_node = head;
// 	while (tmp_node)
// 	{
// 		if (tmp_node->file_info.type == TYPE_CHR || tmp_node->file_info.type == TYPE_BLOCK)
// 			count = parse_len_size_t(tmp_node->file_info.major);
// 		else
// 		{
// 			tmp_node = tmp_node->next;
// 			continue;
// 		}
// 		if (count > max_len)
// 			max_len = count;
// 		tmp_node = tmp_node->next;
// 	}
// 	return (max_len);
// }

// static size_t parse_link_max_length(t_ast *head)
// {
// 	size_t max_len = 0;

// 	t_ast *tmp_node = head;
// 	while (tmp_node)
// 	{
// 		size_t count = parse_len_size_t(tmp_node->file_info.link);
// 		if (count > max_len)
// 			max_len = count;
// 		tmp_node = tmp_node->next;
// 	}
// 	return (max_len);
// }

// int parse_acl(t_ast *head)
// {
// 	t_ast *tmp_node = head;
// 	while (tmp_node)
// 	{
// 		if (tmp_node->file_info.acl_char != ' ')
// 			return (1);
// 		tmp_node = tmp_node->next;
// 	}
// 	return (0);
// }

t_columns	*parse_columns(t_data *data, t_ast *node)
{
	t_columns *columns = ft_calloc(1, sizeof(t_columns));

	if (!data)
		return (NULL);
	(void) node;
	columns->user_max_len = parse_user_max_length(data, node->head);
	columns->group_max_len = parse_group_max_length(data, node->head);
	// columns->size_max_len = parse_size_max_length(node->head);
	// columns->minor_max_len = parse_minor_max_length(node->head);
	// columns->major_max_len = parse_major_max_length(node->head);
	// columns->link_max_len = parse_link_max_length(node->head);
	// columns->as_acl = parse_acl(node->head);

	return (columns);
}