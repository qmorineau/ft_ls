#include "ft_ls.h"

int dirent_type_parser(struct dirent *entry)
{
	switch (entry->d_type)
	{
		case DT_FIFO:
			return TYPE_PIPE;
		case DT_CHR:
			return TYPE_CHR;
		case DT_DIR:
			return TYPE_DIR;
		case DT_BLK:
			return TYPE_BLOCK;
		case DT_REG:
			return TYPE_FILE;
		case DT_LNK:
			return TYPE_LINK;
		case DT_SOCK:
			return TYPE_SOCKET;
		default:
			return TYPE_UNKNOWN;
	}
}

int stat_type_parser(struct stat *buff)
{
	switch (buff->st_mode & S_IFMT)
	{
		case S_IFIFO:
			return TYPE_PIPE;
		case S_IFCHR:
			return TYPE_CHR;
		case S_IFDIR:
			return TYPE_DIR;
		case S_IFBLK:
			return TYPE_BLOCK;
		case S_IFREG:
			return TYPE_FILE;
		case S_IFLNK:
			return TYPE_LINK;
		case S_IFSOCK:
			return TYPE_SOCKET;
		default:
			return TYPE_UNKNOWN;
	}
}