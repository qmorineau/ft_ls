#include "ft_ls.h"

// Globals for opti
static int	g_it = 0;
static char g_print_buff[BUFF_SIZE];
static char g_buff[BUFF_SIZE];

// ******************** Buffering ********************

// Print Buffer

void g_flush()
{
    if (g_it > 0)
	{
		ssize_t res = write(1, g_print_buff, g_it);
		(void) res;
		g_it = 0;
	}
}

inline static void flush()
{
	if (g_it > 0)
	{
		ssize_t res = write(1, g_print_buff, g_it);
		(void) res;
		g_it = 0;
	}
}

inline static void fill_buff(char *str, size_t len)
{
	if (len > BUFF_SIZE / 2)
	{
		flush();
		len = PATH_MAX;
		ssize_t res = write(1, str, len);
		(void) res;
		return ;
	}

	if (g_it + len >= BUFF_SIZE)
		flush();

	ft_memcpy(g_print_buff + g_it, str, len);
	g_it += len;
}

void g_fill_buff_char(char c)
{
	if (g_it + 1 >= BUFF_SIZE)
		flush();
	g_print_buff[g_it++] = c;
}

inline static void fill_buff_char(char c)
{
	if (g_it + 1 >= BUFF_SIZE)
		flush();
	g_print_buff[g_it++] = c;
}

static void fill_buff_file_name(t_data *data, t_file *file)
{
	char *str;
	char *name = get_name(file);

	if (!data->term.is_tty || data->color_parse_error)
		fill_buff(name, ft_strlen(name));
	else
	{
		t_map *tmp = get_colors(data->file_colors, data->colors, file);
		if (tmp)
			str = tmp->value;
		else
			str = "0";
		fill_buff("\e[", 2);
		fill_buff(str, ft_strlen(str));
		fill_buff_char('m');
		fill_buff(name, ft_strlen(name));
		fill_buff("\e[0m", 4);
	}
}

// Other Buffer

static void put_str_buff(char *str, size_t len)
{
	size_t str_len = ft_strlen(str);
	
	if (len)
	{
		size_t space_nbr = len - str_len;
		ft_strlcpy(g_buff, str, BUFF_SIZE);
		ft_memset(&g_buff[str_len], 32, space_nbr);
		fill_buff(g_buff, len);
	}
	else
		fill_buff(str, str_len);
	fill_buff_char(' ');
}

static size_t put_size_t_buff(size_t n, size_t len)
{
	if (!len)
	{
		size_t tmp = n;
		while (tmp >= 10)
		{
			len++;
			tmp /= 10;
		}
		len++;
	}

	ft_memset(g_buff, 32, len);
	int i = len - 1;
	while (n >= 10)
	{
		g_buff[i--] = (n % 10) + 48;
		n /= 10;
	}
	if (i >= 0)
		g_buff[i] = n + 48;
	return len;
}

// ******************** End Buffering ********************

// ******************** Utils ********************

static size_t get_total_blocks(t_ast **array)
{
	size_t blocks = 0;
	for (int i = 0; array[i]; i++)
		blocks += array[i]->file_info.sb.st_blocks;
	return (blocks / 2);
}

// ******************** Print File ********************

void print_file(t_file file, t_data *data, t_columns *columns)
{
	if (data->flags.l || data->flags.g)
	{
		fill_buff(file.permissions, 10);
		if (columns->acl)
			fill_buff_char(file.acl_char);
		fill_buff_char(' ');
		fill_buff(g_buff, put_size_t_buff(file.sb.st_nlink, columns->link));
		fill_buff_char(' ');
		if (!data->flags.g)
			put_str_buff(map_get(data->user_id, &file.sb.st_uid)->value, columns->user);
		put_str_buff(map_get(data->group_id, &file.sb.st_gid)->value, columns->group);
		if (file.type == TYPE_BLOCK || file.type == TYPE_CHR)
		{	
			fill_buff(g_buff, put_size_t_buff(major(file.sb.st_rdev), columns->major));
			fill_buff(", ", 2);
			fill_buff(g_buff, put_size_t_buff(minor(file.sb.st_rdev), columns->minor));
		}
		else
			fill_buff(g_buff, put_size_t_buff(file.sb.st_size, columns->size));
		fill_buff_char(' ');
		fill_buff(file.time_buff, 12);
		fill_buff_char(' ');
		fill_buff_file_name(data, &file);
		if (file.type == TYPE_LINK)
		{
			fill_buff(" -> ", 4);
			fill_buff_file_name(data, file.redirect_file);
		}
	}
	else
		fill_buff_file_name(data, &file);
}

void print_folder_files_list(t_data *data, t_ast **array, t_columns *columns)
{
	for (int i = 0; array[i]; i++)
	{
		if (!array[i]->file_info.error)
			print_file(array[i]->file_info, data, columns);
		if (array[i + 1])
		{
			if (data->flags.l || data->flags.g || !data->term.is_tty)
				fill_buff_char('\n');
			else
				fill_buff("  ", 2);
		}
		else
			fill_buff_char('\n');
	}
}

void print_header(t_data *data, t_ast **array, t_file *file, int print_path)
{
	if (!data->first_print)
		fill_buff_char('\n');
	if ((print_path && data->flags.R && !data->flags.d) || (print_path == 2 && !data->flags.d))
	{
		fill_buff(data->path, data->path_len);
		fill_buff(":\n", 2);
	}
	if (file->error)
	{
		flush();
		strerror(file->error);
	}
	else if ((data->flags.l || data->flags.g) && !data->flags.d )
	{
		fill_buff("total ", 6);
		// flush();
		fill_buff(g_buff, put_size_t_buff(get_total_blocks(array), 0));
		fill_buff_char('\n');
	}
}
