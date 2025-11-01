#include "ft_ls.h"

int parse_time(t_data *data, t_file *file)
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

t_file *parse_link(struct stat *sb, char *path)
{
	struct stat buff;

	t_file *link = ft_calloc(1, sizeof(t_file));
	if (!link)
		return (NULL);
	ssize_t	bufsize = sb->st_size + 1;
	if (sb->st_size == 0)
		bufsize = 256;
	link->name_type = PTR;
	link->name.ptr = ft_calloc(bufsize, sizeof(char));
	if (!link->name.ptr)
	{
		free(link);
		return (NULL);
	}
	ssize_t nbytes = readlink(path, link->name.ptr, bufsize);
	if (nbytes == -1)
	{
		perror("ft_ls");
		free(link);
		return (NULL);
	}
	if (stat(path, &buff) == 0)
		link->type = stat_type_parser(&buff);
	else
		link->type = TYPE_BROKEN_LINK;
	return link;
}

void parse_file_from_stat(t_data *data, t_file *file)
{
	parse_permissions(&file->sb, file);

	// file->redirect_file = parse_link();

	if (data->flags.u)
		file->time = file->sb.st_atime;
	else
		file->time = file->sb.st_mtime;

	char *tmp;
	if (!map_get_id(data->user_id, file->sb.st_uid))
	{
		struct passwd *pw = getpwuid(file->sb.st_uid);
		tmp = ft_strdup(pw->pw_name);
		map_set_uid(&data->user_id, file->sb.st_uid, &tmp);
	}
	if (!map_get_id(data->group_id, file->sb.st_gid))
	{
		struct group *gr = getgrgid(file->sb.st_gid);
		tmp = ft_strdup(gr->gr_name);
		map_set_uid(&data->group_id, file->sb.st_gid, &tmp);
	}
	// if (map_get(data->user_id, sb.st_uid))
	// file->gr = getgrgid(file->sb.st_gid);
	// if (!file->pw || file->gr)
	// {
	// 	// manage error
	// }
	if (parse_time(data, file))
		free_all_and_exit(&data, 2);
}
