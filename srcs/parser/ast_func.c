#include "ft_ls.h"

void ast_clear(t_ast *node)
{
	if (node && node->file_info.redirect_file)
		free(node->file_info.redirect_file);
}
