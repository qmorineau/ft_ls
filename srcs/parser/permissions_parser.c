#include "ft_ls.h"

static const char type[] = "-dlbpsc-";

static void put_special_bits(t_file *file, short dec)
{
	int special = (dec >> 9) & 0b111;

	if (special & 0b100)
		file->permissions[3] = file->permissions[3] == 'x' ? 's' : 'S';
	if (special & 0b010)
		file->permissions[6] = file->permissions[6] == 'x' ? 's' : 'S';
	if (special & 0b001)
		file->permissions[9] = file->permissions[9] == 'x' ? 't' : 'T';
}

static  void put_basic_permissions(t_file *file, short dec)
{
	int i = 1;
	for (int shift = 6; shift >= 0; shift -= 3)
	{
		short group = (dec >> shift) & 0b111;
		file->permissions[i++] = (group & 0b100) ? 'r' : '-';
		file->permissions[i++] = (group & 0b010) ? 'w' : '-';
		file->permissions[i++] = (group & 0b001) ? 'x' : '-';
	}
}

void parse_permissions(struct statx *buff, t_file *file)
{
	short perm = buff->stx_mode & 07777; // Bits suppression to keep only permissions bits

	memset(file->permissions, '-', 10);
	file->permissions[10] = 0;

	file->permissions[0] = type[file->type];
	put_basic_permissions(file, perm);
	put_special_bits(file, perm);
}