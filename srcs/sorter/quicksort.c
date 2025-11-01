#include "ft_ls.h"

static void swap(t_ast **ptr1, t_ast **ptr2)
{
	t_ast *tmp;

	tmp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = tmp;
}

static void quicksort(t_ast **arr, int left, int right, int (*f)(t_ast *, t_ast *))
{
	if (left < right)
	{
		int i = left - 1;
		int j = right + 1;
		t_ast *pivot = arr[(left + right) / 2];

		while (++i <= --j)
		{
			while (f(pivot, arr[i])) i++;  // move i right
			while (f(arr[j], pivot)) j--;  // move j left
			if (i <= j)
			{
				if (f(arr[i], arr[j]))
					swap(&arr[i], &arr[j]);
			}
			else
				break;
		}
		quicksort(arr, left, j, f);
		quicksort(arr, i, right, f);
	}
}

static int sort_ascii(t_ast *node1, t_ast *node2)
{
	const char *name1 = get_name(&node1->file_info);
	int res = ft_strncmp(name1, get_name(&node2->file_info), ft_strlen(name1));
	if (res > 0)
		return 1;
	return 0;
}

static int sort_recently(t_ast *node1, t_ast *node2)
{
	if (node1->file_info.time < node2->file_info.time)
		return 1;
	else if (node1->file_info.time == node2->file_info.time)
	{
		if (node1->index > node2->index)
			return 1;
		return 0;
	}
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
			quicksort(*array, 0, len - 1, sort_ascii);
		else
			quicksort(*array, 0, len - 1, sort_recently);		
	}
	else if (flags.f)
		return ;
	else if (flags.t)
		quicksort(*array, 0, len - 1, sort_recently);
	else
		quicksort(*array, 0, len - 1, sort_ascii);
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
