#include "ft_ls.h"

static const char type[] = "dlbpsc-";

inline static void put_special_bits(t_file *file, char octal_permission[5])
{
	int special_bits = octal_permission[0] - 48;
	if (special_bits >= 4)
	{
		special_bits -= 4;
		if (file->permissions[3] == 'x')
			file->permissions[3] = 's';
		else
			file->permissions[3] = 'S';
	}
	if (special_bits >= 2)
	{
		special_bits -= 2;
		if (file->permissions[6] == 'x')
			file->permissions[6] = 's';
		else
			file->permissions[6] = 'S';
	}
	if (special_bits >= 1)
	{
		special_bits -= 1;
		if (file->permissions[9] == 'x')
			file->permissions[9] = 't';
		else
			file->permissions[9] = 'T';
	}
}

inline static void put_basic_permissions(t_file *file, short dec)
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

inline static void put_permissions(t_file *file, char octal_permission[5], short dec)
{
	memset(file->permissions, '-', 10);
	file->permissions[10] = 0;

	file->permissions[0] = type[file->type];
	put_basic_permissions(file, dec);
	put_special_bits(file, octal_permission);
}

inline void parse_permissions(struct stat *buff, t_file *file)
{
	char octal_perm[5];

	unsigned int decimal = buff->st_mode & 07777; // Bits suppression to keep only permissions bits

	for (int i = 3; i >= 0; i--)
	{
		unsigned int rest = decimal % 8;
		octal_perm[i] = (char) rest + 48;
		decimal = decimal / 8;
	}
	octal_perm[4] = 0;

	decimal = buff->st_mode & 07777; // Bits suppression to keep only permissions bits
	put_permissions(file, octal_perm, decimal);
}