#include "ft_ls.h"

char *get_name(t_file *file)
{
	if (file->name_type == BUFFER)
		return (file->name.buff);
	else
		return (file->name.ptr);
}