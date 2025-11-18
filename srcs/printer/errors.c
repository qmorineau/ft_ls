#include "ft_ls.h"

void print_error(t_data *data, t_file file)
{
	g_flush();
	if (file.type == TYPE_DIR)
	{
		fill_buff_error("/bin/ls: cannot open directory '", 32);
		// fill_buff_error("ft_ls: cannot open directory '", 30);
	}
	else
	{
		fill_buff_error("/bin/ls: cannot access '", 24);
		// fill_buff_error("ft_ls: cannot access '", 22);
	}
	fill_buff_error(data->path, data->path_len);
	fill_buff_error("'", 1);
	errno = file.error;
	perror_print_buff();
}
