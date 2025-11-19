#include "ft_ls.h"

char get_acl(char *path)
{
	acl_t acl = acl_get_file(path, ACL_TYPE_ACCESS);
	if (!acl)
		return (' ');
	int count = 0;
    acl_entry_t entry;
    int entry_id = ACL_FIRST_ENTRY;
    while (acl_get_entry(acl, entry_id, &entry) == 1) {
        count++;
        entry_id = ACL_NEXT_ENTRY;
    }
    acl_free(acl);
	if (count > 3)
		return ('+');
	else
		return (' ');
}

char	get_ext_attr(char *path)
{
	return (' ');

	ssize_t size = listxattr(path, NULL, 0);
	if (size > 0)
		return ('@');
	return (' ');
}