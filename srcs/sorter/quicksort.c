#include "ft_ls.h"

void swap(t_ast **ptr1, t_ast **ptr2)
{
	t_ast *tmp;

	tmp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = tmp;
}

void quicksort(t_ast **arr, int left, int right, int (*f)(t_ast *, t_ast *))
{
	if (left < right)
	{
		int i = left;
		int j = right;
		t_ast *pivot = arr[(left + right) / 2];

		while (i <= j)
		{
			while (f(pivot, arr[i])) i++;  // move i right
			while (f(arr[j], pivot)) j--;  // move j left
			if (i <= j)
				swap(&arr[i++], &arr[j--]);
		}
		quicksort(arr, left, j, f);
		quicksort(arr, i, right, f);
	}
}

int sort_alphabetically(t_ast *node1, t_ast *node2)
{
	char *name1 = ft_strdup(node1->file_info.name[0] == '.' ? &node1->file_info.name[1] : node1->file_info.name);
	char *name2 = ft_strdup(node2->file_info.name[0] == '.' ? &node2->file_info.name[1] : node2->file_info.name);

	//check name1 et name2

	for (int i = 0; name1[i]; i++)
		name1[i] = ft_tolower(name1[i]);
	for (int i = 0; name2[i]; i++)
		name2[i] = ft_tolower(name2[i]);
	int res = ft_strncmp(name1, name2, strlen(name1));
	free(name1);
	free(name2);
	if (res > 0)
		return 1;
	return 0;
}

int sort_recently(t_ast *node1, t_ast *node2)
{
	printf("%s %zu < %zu %s\n", node1->file_info.name ,node1->file_info.raw_time, node2->file_info.raw_time, node2->file_info.name);
	if (node1->file_info.raw_time < node2->file_info.raw_time)
		return 1;
	return 0;
}

int test_ascii(t_ast *node1, t_ast *node2)
{
	// printf("n1 = %s\n n2 = %s\n", node1->file_info.name, node2->file_info.name);
	int res = ft_strncmp(node1->file_info.name, node2->file_info.name, strlen(node1->file_info.name));
	// printf("res = %d %c\n", res, res);
	if (res > 0)
		return 1;
	return 0;
}

void sort_array(t_ast ***array, t_flags flags)
{
	int len = 0;
	while ((*array)[len])
		len++;
	if (flags.u)
	{
	// -u     with -lt: sort by, and show, access time; with -l: show access time and sort by name; otherwise: sort by access time, newest first
		quicksort(*array, 0, len - 1, test_ascii);
	}
	else if (flags.f)
		return ;
	else if (flags.t)
		quicksort(*array, 0, len - 1, sort_recently);
	else
		quicksort(*array, 0, len - 1, sort_alphabetically);

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