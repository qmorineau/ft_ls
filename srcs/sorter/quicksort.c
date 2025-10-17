#include "ft_ls.h"

void quicksort(t_ast **arr, int left, int right) {
    if (left < right) {
        int pivot = arr[(left + right) / 2];
        int i = left, j = right;

        while (i <= j) {
            while (arr[i] <= pivot && i <= right) i++;
            while (arr[j] >= pivot && j >= left) j--;
            if (i <= j) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
                i++;
                j--;
            }
        }
        quicksort(arr, left, j);
        quicksort(arr, i, right);
    }
}