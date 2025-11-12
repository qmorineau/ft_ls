#include "ft_ls.h"

static unsigned int	g_it = 0;
static char g_buff_error[BUFF_SIZE];

void perror_print_buff()
{
	g_buff_error[g_it] = 0;
	perror(g_buff_error);
	g_it = 0;
}

void flush_error()
{
	if (g_it > 0)
	{
		ssize_t res = write(2, g_buff_error, g_it);
		(void) res;
		g_it = 0;
	}
}

void fill_buff_error(char *str, size_t len)
{
	if (len > BUFF_SIZE / 2)
	{
		flush_error();
		ssize_t res = write(2, str, len);
		(void) res;
		return ;
	}

	if (g_it + len >= BUFF_SIZE)
		flush_error();

	ft_memcpy(g_buff_error + g_it, str, len);
	g_it += len;
	g_buff_error[g_it] = 0;
}