#include "ft_ls.h"

char *get_name(t_file *file)
{
	switch (file->name_type)
	{
		case E_PATH:
			return (file->name.path); 
		case E_PTR:
			return (file->name.ptr);
		default:
			return (file->name.buff);
	}
}
