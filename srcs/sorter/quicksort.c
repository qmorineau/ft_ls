#include "ft_ls.h"

// void quicksort(t_ast **arr, int left, int right) {
//     if (left < right) {
//         int pivot = arr[(left + right) / 2];
//         int i = left, j = right;

//         while (i <= j) {
//             while (arr[i] <= pivot && i <= right) i++;
//             while (arr[j] >= pivot && j >= left) j--;
//             if (i <= j) {
//                 int temp = arr[i];
//                 arr[i] = arr[j];
//                 arr[j] = temp;
//                 i++;
//                 j--;
//             }
//         }
//         quicksort(arr, left, j);
//         quicksort(arr, i, right);
//     }
// }

void swap(t_ast **ptr1, t_ast **ptr2)
{
	printf("swaping\n");
	t_ast *tmp;

	tmp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = tmp;
}

// void quicksort(t_ast ***array, int left, int right, int (*f)(t_ast *, t_ast *))
// {
// 	t_ast **arr = *array;

// 	if (left < right)
// 	{
// 		// int pivot = (left + right) / 2;
// 		// printf("+ left = %d\n pivot = %d\n right = %d\n", left, pivot, right);
// 		int i = left;
// 		int j = right;

// 		printf("\n\narr[%d] = %s\narr[%d] = %s\n\n", i, arr[i]->file_info.name, j, arr[j]->file_info.name);
// 		printf("res = %d\n", f(arr[i], arr[j]));
// 		while (f(arr[i], arr[j]) && i != j)
// 		{
// 			// while (f(arr[i], arr[pivot]) && i <= right) i++;
// 			// while (f(arr[j], arr[pivot]) && j >= left) j--;
// 			printf("\n\nswap arr[%d] = %s\narr[%d] = %s\n\n", i, arr[i]->file_info.name, j, arr[j]->file_info.name);
// 			if (f(arr[i], arr[j]) && i != j)
// 				swap(&arr[i++], &arr[j--]);
// 		}
// 		// printf("left = %d\n pivot = %d\n right = %d\n", left, pivot, right);
// 		quicksort(array, left, j, f);
// 		quicksort(array, i, right, f);
// 	}
// }

// void quicksort(t_ast ***array, int left, int right, int (*f)(t_ast *, t_ast *))
// {
// 	t_ast **arr = *array;
// 	if (left < right)
// 	{
// 		int pi = 
// 	}
// }

int test_ascii(t_ast *node1, t_ast *node2)
{
	// printf("n1 = %s\n n2 = %s\n", node1->file_info.name, node2->file_info.name);
	int res = ft_strncmp(node1->file_info.name, node2->file_info.name, strlen(node1->file_info.name));
	// printf("res = %d %c\n", res, res);
	if (res > 0)
		return 1;
	return 0;
}

void sort_array(t_ast ***array, int (*f)(t_ast *, t_ast *))
{
	int len = 0;
	while ((*array)[len])
		len++;
	// printf("len = %d\n", len);

	(void) f;
	// quicksort(array, 0, len - 1, f);
}