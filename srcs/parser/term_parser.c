#include "ft_ls.h"

void	parse_terminal(t_terminfo *term_struct)
{
	if (isatty(STDOUT_FILENO))
	{
		term_struct->is_tty = 1;
		
		struct winsize w;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0)
			term_struct->width = w.ws_col;
	}
	else
		term_struct->is_tty = 0;
}
