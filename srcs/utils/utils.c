#include "ft_ls.h"

char *get_name(t_file *file)
{
	if (file->name_type == E_PATH)
		return (file->name.path);
	else
		return (file->name.buff);
}