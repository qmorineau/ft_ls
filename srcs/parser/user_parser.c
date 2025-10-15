#include "ft_ls.h"

void get_user(uid_t uid)
{
	struct passwd *user = getpwuid(uid);
	(void) user;
}