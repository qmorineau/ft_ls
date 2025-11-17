#include "ft_ls.h"

static int parse_time(t_data *data, t_file *file)
{
	time_t t = (time_t) file->time.tv_sec;
	char *str = ctime(&t);
	if (!str)
	{
		file->error = errno;
		perror("ft_ls");
		return (1);
	}
	if (data->now - file->time.tv_sec > MONTH_IN_SEC * 6)
	{
		ft_strlcpy(file->time_buff, &str[4], 7);
		ft_memset(&file->time_buff[6], ' ', 2);
		ft_strlcpy(&file->time_buff[8], &str[20], 5);
	}
	else
	{
		// ft_strlcpy(file->access_time, str, 13); /* laptop */
		ft_strlcpy(file->time_buff, &str[4], 13); /* school */
	}
	return (0);
}

t_file *parse_link(t_data *data)
{
	t_file *link = malloc(sizeof(t_file));
	if (!link)
		return (NULL);
	link->redirect_file = NULL;
	link->name_type = 0;
	ssize_t	bufsize = PATH_MAX - 1;
	link->name_type = E_PATH;
	ssize_t nbytes = readlink(data->path, link->name.path, bufsize);
	if (nbytes == -1)
	{
		fill_buff_error("ft_ls: cannot read symbolic link '", 34);
		fill_buff_error(data->path, data->path_len);
		fill_buff_error("'", 1);
		perror_print_buff();
		free(link);
		return (NULL);
	}
	link->name.path[nbytes] = '\0';
	if (statx(AT_FDCWD, data->path, AT_SYMLINK_FOLLOW, data->stax_mask, &link->sb) == 0)
		link->type = stat_type_parser(&link->sb);
	else
		link->type = TYPE_BROKEN_LINK;
	return link;
}

static int parse_group_name(t_data *data, t_file *file)
{
	char *tmp;
	if (!map_get(data->group_id, &file->sb.stx_gid))
	{
		struct group *gr = getgrgid(file->sb.stx_gid);
		if (gr)
		{
			tmp = ft_strdup(gr->gr_name);
			if (!tmp)	
				return (0);
			if (!map_set(&data->group_id, &file->sb.stx_gid, tmp, UID))
				return (free(tmp), 0);
		}
	}
	return (1);
}

static int parse_user_name(t_data *data, t_file *file)
{
	char *tmp;
	if (!map_get(data->user_id, &file->sb.stx_uid))
	{
		struct passwd *pw = getpwuid(file->sb.stx_uid);
		if (pw)
		{
			tmp = ft_strdup(pw->pw_name);
			if (!tmp)
				return (0);
			if(!map_set(&data->user_id, &file->sb.stx_uid, tmp, UID))
				return (free(tmp), 0);
		}
	}
	return (1);
}

int parse_file_from_stat(t_data *data, t_file *file)
{
	parse_permissions(&file->sb, file);

	struct statx_timestamp *time = NULL;

	if (data->flags.l || data->flags.g)
	{
		if (file->type == TYPE_LINK)
		{
			file->acl_char = get_acl(get_name(file->redirect_file));
			file->ext_attr_char = get_ext_attr(get_name(file->redirect_file));
		}
		else
		{
			file->acl_char = get_acl(data->path);
			file->ext_attr_char = get_ext_attr(data->path);
		}
		if (data->flags.u)
			time = &file->sb.stx_atime;
		else
			time = &file->sb.stx_mtime;
		if (!parse_user_name(data, file) || !parse_group_name(data, file))
			return (0);
		if (time)
			{ file->time.tv_nsec = time->tv_nsec; file->time.tv_sec = time->tv_sec; }
		if (parse_time(data, file))
			free_all_and_exit(data, 2);
	}
	else if (data->flags.u)
		time = &file->sb.stx_atime;
	else if (data->flags.t)
		time = &file->sb.stx_mtime;
	if (time)
		{ file->time.tv_nsec = time->tv_nsec; file->time.tv_sec = time->tv_sec; }
	return (1);
}
