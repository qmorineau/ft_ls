#include "ft_ls.h"

static void swap(t_ast **ptr1, t_ast **ptr2)
{
	t_ast *tmp;

	tmp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = tmp;
}

static void quicksort(t_ast **arr, int left, int right, int (*f)(t_ast *, t_ast *, int option), int is_args)
{
	if (left < right)
	{
		int i = left - 1;
		int j = right + 1;
		t_ast *pivot = arr[(left + right) / 2];

		while (++i <= --j)
		{
			while (f(pivot, arr[i], is_args)) i++;  // move i right
			while (f(arr[j], pivot, is_args)) j--;  // move j left
			if (i <= j)
			{
				if (f(arr[i], arr[j], is_args))
					swap(&arr[i], &arr[j]);
			}
			else
				break;
		}
		quicksort(arr, left, j, f, is_args);
		quicksort(arr, i, right, f, is_args);
	}
}

static int sort_files_type(t_file file1, t_file file2)
{
	if (file1.type == file2.type)
		return (-1);
	else if (file1.type == TYPE_DIR)
		return (1);
	else
		return (0);
}

static int sort_ascii(t_ast *node1, t_ast *node2, int is_args)
{
	if (is_args)
	{
		switch (sort_files_type(node1->file_info, node2->file_info))
		{
			case 0:
				return (0);
			case 1:
				return (1);
			default:
				break;
		}
	}
	const char *name1 = get_name(&node1->file_info);
	int res = ft_strncmp(name1, get_name(&node2->file_info), ft_strlen(name1));
	if (res > 0)
		return 1;
	return 0;
}

static int sort_recently(t_ast *node1, t_ast *node2, int is_args)
{
	if (is_args)
	{
		switch (sort_files_type(node1->file_info, node2->file_info))
		{
			case 0:
				return (0);
			case 1:
				return (1);
			default:
				break;
		}
	}
	if (node1->file_info.time < node2->file_info.time)
		return 1;
	else if (node1->file_info.time == node2->file_info.time)
		return (sort_ascii(node1, node2, is_args));
	return 0;
}

void sort_array(t_ast ***array, t_flags flags)
{
	int len = 0;
	while ((*array)[len])
		len++;
	if (flags.u)
	{
		if (flags.l && !flags.t)
			quicksort(*array, 0, len - 1, sort_ascii, 0);
		else
			quicksort(*array, 0, len - 1, sort_recently, 0);		
	}
	else if (flags.f)
		return ;
	else if (flags.t)
		quicksort(*array, 0, len - 1, sort_recently, 0);
	else
		quicksort(*array, 0, len - 1, sort_ascii, 0);
	// Reverse order
	if (flags.r)
	{
		int i = 0;
		while ((*array)[i + 1])
			i++;
		for (int j = 0; j < i; j++)
			swap(&(*array)[j], &(*array)[i--]);
	}
}

void sort_array_args(t_ast ***array, t_flags flags)
{
	int len = 0;
	while ((*array)[len])
		len++;
	if (flags.u)
	{
		if (flags.l && !flags.t)
			quicksort(*array, 0, len - 1, sort_ascii, 1);
		else
			quicksort(*array, 0, len - 1, sort_recently, 1);		
	}
	else if (flags.f)
		return ;
	else if (flags.t)
		quicksort(*array, 0, len - 1, sort_recently, 1);
	else
		quicksort(*array, 0, len - 1, sort_ascii, 1);
	// Reverse order
	if (flags.r)
	{
		int i = 0;
		while ((*array)[i + 1])
			i++;
		for (int j = 0; j < i; j++)
			swap(&(*array)[j], &(*array)[i--]);
	}
}
