#include "ft_ls.h"

static void put_special_bits(t_file *file, char octal_permission[5])
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

static void put_type(t_file *file)
{
	switch (file->type)
	{
		case TYPE_DIR:
			file->permissions[0] = 'd';
			break;
		case TYPE_LINK:
			file->permissions[0] = 'l';
			break;
		case TYPE_BLOCK:
			file->permissions[0] = 'b';
			break;
		case TYPE_PIPE:
			file->permissions[0] = 'p';
			break;
		case TYPE_SOCKET:
			file->permissions[0] = 's';
			break;
		case TYPE_CHR:
			file->permissions[0] = 'c';
			break;
		default:
			break;
	}
}

static void put_basic_permissions(t_file *file, char octal_permission[5])
{
	for (int i = 0; i < 3; i++)
	{
		int nbr = octal_permission[i + 1] - 48;
		if (nbr >= 4)
		{
			nbr -= 4;
			file->permissions[3 * i + 1] = 'r';
		}
		if (nbr >= 2)
		{
			nbr -= 2;
			file->permissions[3 * i + 2] = 'w';
		}
		if (nbr >= 1)
			file->permissions[3 * i + 3] = 'x';
	}
}

static void put_permissions(t_file *file, char octal_permission[5])
{
	memset(file->permissions, '-', 10);
	file->permissions[10] = 0;

	put_type(file);
	put_basic_permissions(file, octal_permission);
	put_special_bits(file, octal_permission);
}

void parse_permissions(struct stat *buff, t_file *file)
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

	put_permissions(file, octal_perm);
}