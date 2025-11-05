#include "ft_ls.h"

static int parse_time(t_data *data, t_file *file)
{
	char *str = ctime(&file->time);
	if (!str)
	{
		perror("ft_ls");
		return (1);
	}
	if (data->now - file->time > MONTH_IN_SEC * 6)
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
		perror("ft_ls");
		free(link);
		return (NULL);
	}
	link->name.path[nbytes] = '\0';
	if (stat(data->path, &link->sb) == 0)
		link->type = stat_type_parser(&link->sb);
	else
		link->type = TYPE_BROKEN_LINK;
	return link;
}

void parse_file_from_stat(t_data *data, t_file *file)
{
	parse_permissions(&file->sb, file);

	if (data->flags.u)
		file->time = file->sb.st_atime;
	else if (data->flags.t)
		file->time = file->sb.st_mtime;

	char *tmp;
	if (data->flags.l || data->flags.g)
	{
		if (file->type == TYPE_LINK)
			file->acl_char = get_acl(get_name(file->redirect_file));
		file->time = file->sb.st_mtime;
		if (!map_get(data->user_id, &file->sb.st_uid))
		{
			struct passwd *pw = getpwuid(file->sb.st_uid);
			tmp = ft_strdup(pw->pw_name);
			if (!tmp)
				exit(2); // manage error
			if(!map_set(&data->user_id, &file->sb.st_uid, tmp, UID))
				exit(2); // manage error
		}
		if (!map_get(data->group_id, &file->sb.st_gid))
		{
			struct group *gr = getgrgid(file->sb.st_gid);
			tmp = ft_strdup(gr->gr_name);
			if (!tmp)
				exit(2); // manage error
			if (!map_set(&data->group_id, &file->sb.st_gid, tmp, UID))
				exit(2); // manage error
		}
		if (parse_time(data, file))
			free_all_and_exit(data, 2);
	}
}
