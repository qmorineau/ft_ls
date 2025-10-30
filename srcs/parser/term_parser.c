#include "ft_ls.h"

void	parse_terminal(t_terminfo *term_struct)
{
	if (isatty(STDOUT_FILENO))
		term_struct->is_tty = 1;
	else
		term_struct->is_tty = 0;
}
