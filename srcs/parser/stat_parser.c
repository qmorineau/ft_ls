#include "ft_ls.h"

int parse_file_type(struct stat *buff)
{
	switch (buff->st_mode & S_IFMT)
	{
		// Folder
		case S_IFDIR:
			return (TYPE_FOLDER);
		// File
		case S_IFREG:
			return (TYPE_FILE);
		// check man stat for more type
		default:
			// handle default ??
			return (-1);
	}
}

int parse_permissions(struct stat *buff)
{
	char buffer[4] = {0};

	unsigned int decimal = buff->st_mode & 07777; // Bits suppression to keep only permissions bits

	(void) decimal;
	return (ft_atoi(buffer));
}

void parse_user()
{

}

void parse_group()
{

}

void parse_time()
{

}