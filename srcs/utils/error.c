#include "ft_ls.h"

void stat_error(char *path)
{
	struct stat sb;
	lstat(path, &sb);
	ft_putstr_fd("ft_ls: cannot acces '", 2);
	ft_putstr_fd(path, 2);
	perror("'");
}

void opendir_error(char *path)
{
	opendir(path);
	ft_putstr_fd("ft_ls: cannot open directory '", 2);
	ft_putstr_fd(path, 2);
	perror("'");
}