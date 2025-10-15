#include "ft_ls.h"

void get_attributes(char *path)
{
	char *list = NULL;
	size_t size = 50;

	ssize_t len = listxattr(path, list, size);
	// printf("coucou len = %zd\n", len);
	write(2, list, len);
} 