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

static void update_user_max_length(t_len_accumulator *acc, t_data *data, t_ast *node)
{
	if(!data->user_id)
		return ;

	t_map_uid *user = map_get_id(data->user_id, node->file_info.sb.st_uid);
	if (user->len > acc->user)
		acc->user = user->len;
}

static void update_group_max_length(t_len_accumulator *acc, t_data *data, t_ast *node)
{
	if(!data->group_id)
		return ;

	t_map_uid *group = map_get_id(data->group_id, node->file_info.sb.st_gid);
	if (group->len > acc->group)
		acc->group = group->len;
}

static void update_size_max_length(t_len_accumulator *acc, t_ast *node)
{
	size_t count;

	if (node->file_info.type == TYPE_CHR || node->file_info.type == TYPE_BLOCK)
	{
		size_t minor = parse_len_size_t(minor(node->file_info.sb.st_rdev));
		size_t major = parse_len_size_t(major(node->file_info.sb.st_rdev));
		if (minor > acc->minor)
			acc->minor = minor;
		if (major > acc->major)
			acc->major = major;
		count = minor + major + 2;
	}
	else
		count = parse_len_size_t(node->file_info.sb.st_size);
	if (count > acc->size)
		acc->size = count;

}


static void update_link_max_length(t_len_accumulator *acc, t_ast *node)
{
	size_t count = parse_len_size_t(node->file_info.sb.st_nlink);

	if (count > acc->link)
		acc->link = count;
}

static void update_acl_len(t_len_accumulator *acc, t_ast *node)
{
	if (node->file_info.acl_char != ' ')
		acc->acl = 1;
}

void parse_columns(t_columns *columns, t_data *data, t_ast *node)
{
	ft_memset(columns, 0, sizeof(t_columns));


	if (data->flags.l || data->flags.g)
	{
		t_len_accumulator acc = {0};

		t_ast *tmp = node->head;
		while (tmp)
		{
			update_user_max_length(&acc, data, tmp);
			update_group_max_length(&acc, data, tmp);
			update_size_max_length(&acc, tmp);
			update_link_max_length(&acc, tmp);
			update_acl_len(&acc, tmp);
			tmp = tmp->next;
		}

		columns->user_max_len = acc.user;
		columns->group_max_len = acc.group;
		columns->size_max_len = acc.size;
		columns->minor_max_len = acc.minor;
		columns->major_max_len = acc.major;
		columns->link_max_len = acc.link;
		columns->as_acl = acc.acl;
	}
}
