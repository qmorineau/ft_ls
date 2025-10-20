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

void parse_permissions(struct stat *buff, char str_buff[1][4])
{
	unsigned int decimal = buff->st_mode & 07777; // Bits suppression to keep only permissions bits

	// printf("deci = %u\n", decimal);
	for (int i = 2; i >= 0; i--)
	{
		unsigned int rest = decimal % 8;
		// printf("%u = rest \n", rest);
		(*str_buff)[i] = (char) rest + 48;
		decimal = decimal / 8;
	}
	(*str_buff)[3] = 0;
	printf("buff = %s\n", (*str_buff));
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