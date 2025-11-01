#include "ft_ls.h"

// static char *parse_user(struct stat *buff)
// {
// 	struct passwd *pw = getpwuid(buff->st_uid);
// 	if (!pw)
// 		return (NULL);
// 	char *name = ft_strdup(pw->pw_name);
// 	if (!name)
// 		return (NULL);
// 	return (name);
// }

// static char *parse_group(struct stat *buff)
// {
// 	struct group *gr = getgrgid(buff->st_gid);
// 	if (!gr)
// 		return (NULL);
// 	char *name = ft_strdup(gr->gr_name);
// 	if (!name)
// 		return (NULL);
// 	return (name);
// }

// int parse_modified_time(t_data *data, t_file *file, struct stat *buff)
// {
// 	char *str = ctime(&buff->st_mtime);
// 	if (!str)
// 	{
// 		perror("ft_ls");
// 		free_all_and_exit(&data, 2);
// 	}
// 	if (data->now - buff->st_mtime > MONTH_IN_SEC * 6)
// 	{
// 		ft_strlcpy(file->mod_time, &str[4], 7);
// 		ft_memset(&file->mod_time[6], ' ', 2);
// 		ft_strlcpy(&file->mod_time[8], &str[20], 5);
// 	}
// 	else
// 	{
// 		// ft_strlcpy(file->access_time, str, 13); /* laptop */
// 		ft_strlcpy(file->mod_time, &str[4], 13); /* school */
// 	}
// 	return (0);
// }

// int parse_access_time(t_data *data, t_file *file, struct stat *buff)
// {
// 	char *str = ctime(&buff->st_atime);
// 	if (!str)
// 	{
// 		perror("ft_ls");
// 		return (1);
// 	}
// 	if (data->now - buff->st_atime > MONTH_IN_SEC * 6)
// 	{
// 		ft_strlcpy(file->access_time, &str[4], 7);
// 		ft_memset(&file->access_time[6], ' ', 2);
// 		ft_strlcpy(&file->access_time[8], &str[20], 5);
// 	}
// 	else
// 	{
// 		// ft_strlcpy(file->access_time, str, 13); /* laptop */
// 		ft_strlcpy(file->access_time, &str[4], 13); /* school */
// 	}
// 	return (0);
// }

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

	file->pw = getpwuid(file->sb.st_uid);
	file->gr = getgrgid(file->sb.st_gid);
	if (!file->pw || file->gr)
	{
		// manage error
	}
	// if (parse_modified_time(data, file, buff) || parse_access_time(data, file, buff))
	// 	free_all_and_exit(&data, 2);
}
