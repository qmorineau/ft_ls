#include "ft_ls.h"

static char *parse_user(struct stat *buff)
{
	struct passwd *pw = getpwuid(buff->st_uid);
	if (!pw)
		return (NULL); //error
	char *name = ft_strdup(pw->pw_name);
	if (!name)
		return (NULL); //error
	return (name);
}

static char *parse_group(struct stat *buff)
{
	struct group *gr = getgrgid(buff->st_gid);
	if (!gr)
		return (NULL); //error
	char *name = ft_strdup(gr->gr_name);
	if (!name)
		return (NULL); //error
	return (name);
}

static char *parse_time(struct stat *buff)
{
	char *str = ctime(&buff->st_mtime);

	return ft_strndup(&str[4], 12);
}

static char *parse_access_time(struct stat *buff)
{
	char *str = ctime(&buff->st_atime);

	return ft_strndup(&str[4], 12);
}

static t_file *parse_link(t_file *file, struct stat *sb, char *path)
{
	struct stat buff;

	t_file *link = ft_calloc(1, sizeof(t_file));
	if (!file)
		return (NULL);
	ssize_t	bufsize = sb->st_size + 1;
	if (sb->st_size == 0)
		bufsize = 256;
	char *name = ft_calloc(bufsize, sizeof(char));
	if (!name)
		return (NULL);
	ssize_t nbytes = readlink(path, name, bufsize);
	if (nbytes == -1)
	{
		// error
	}
	strncpy(link->name, name, 256);
	if (stat(path, &buff) == 0)
		link->type = stat_type_parser(&buff);
	else
		link->type = TYPE_BROKEN_LINK;
	free(name);
	return link;
}

int parse_file_from_stat(t_file *file, struct stat *buff, char *path)
{
	if (file->type == TYPE_LINK)
	{
		file->redirect_file = parse_link(file, buff, path);
		if (!file->redirect_file)
		{
			//error
		}
	}
	// get everything
	file->size = buff->st_size;
	file->block_size = buff->st_blocks;
	parse_permissions(buff, file);
	file->user_name = parse_user(buff);
	//check res
	file->group_name = parse_group(buff);
	// check res
	file->link = buff->st_nlink;
	file->mod_time = parse_time(buff);
	file->access_time = parse_access_time(buff);
	file->raw_mod_time = buff->st_mtime;
	file->raw_access_time = buff->st_atime;
	if (file->type == TYPE_BLOCK || file->type == TYPE_CHR)
	{
		unsigned int device = buff->st_rdev;
		file->major = (device >> 8) & 0xfff; // get the value of major device, same as major() macro
		file->minor = (device & 0xff) | ((device >> 12) & 0xfff00); // get the value of minor device, same as minor() macro
	}
	// Check res
	return (0);
}