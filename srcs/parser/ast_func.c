#include "ft_ls.h"

void ast_clear(t_ast *node)
{
	if (node)
		free_file_info(&node->file_info);
}
