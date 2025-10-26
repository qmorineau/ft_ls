#include "ft_ls.h"

int parse_file_type(struct stat *buff)
{
	switch (buff->st_mode & S_IFMT)
	{
		// Folder
		case S_IFDIR:
			return (TYPE_DIR);
		// File
		case S_IFREG:
			return (TYPE_FILE);
		// check man stat for more type symlink
		default:
			// handle default ??
			return (-1);
	}
}

void parse_permissions(struct stat *buff, char str_buff[1][5])
{
	unsigned int decimal = buff->st_mode & 07777; // Bits suppression to keep only permissions bits

	for (int i = 3; i >= 0; i--)
	{
		unsigned int rest = decimal % 8;
		(*str_buff)[i] = (char) rest + 48;
		decimal = decimal / 8;
	}
	(*str_buff)[4] = 0;
}

char *parse_user(struct stat *buff)
{
	struct passwd *pw = getpwuid(buff->st_uid);
	if (!pw)
		return (NULL); //error
	char *name = ft_strdup(pw->pw_name);
	if (!name)
		return (NULL); //error
	return (name);
}

char *parse_group(struct stat *buff)
{
	struct group *gr = getgrgid(buff->st_gid);
	if (!gr)
		return (NULL); //error
	char *name = ft_strdup(gr->gr_name);
	if (!name)
		return (NULL); //error
	return (name);
}

char *parse_time(struct stat *buff)
{
	char *str = ctime(&buff->st_mtime);

	return ft_strndup(&str[4], 12);
}

char *parse_access_time(struct stat *buff)
{
	char *str = ctime(&buff->st_atime);

	return ft_strndup(&str[4], 12);
}