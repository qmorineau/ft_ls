#include "ft_ls.h"

char *get_name(t_file *file)
{
	if (file->name_type == E_PATH)
		return (file->name.path);
	else if (file->name_type == E_PTR)
		return (file->name.ptr);
	else
		return (file->name.buff);
}