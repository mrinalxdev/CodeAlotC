// Bubble Sort

/*Workflow
> Starts at the beginning of the list
> Compare the first two elements. If they are in the wrong order, swap them.
> Move to the next pair of elements (second and third). If they are in the wrong order, swap them.
> Repeat step 3 until the end of the list is reached.
> Repeat steps 1-4 until no more swaps are needed, indicating the list is sorted.
*/

#include <stdio.h>

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void bubbleSort(int arr[], int n){
    int i, j;
    for (i = 0; i < n - 1; i ++){
        for(j = 0; j < n - i -1; j++){
            if (arr[j] > arr[j + 1]){
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

void printArray(int arr[], int n){
    int i;
    for (i = 0; i < n; i++){
        printf("%d", arr[i]);
    }
    printf("\n");
}

int main(){
    int arr[] = { 64, 4, 26, 2, 67}
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Original arrat : ");
    printArray(arr, n);

    return 0;
}

// Selection Sort

/* Workflow
>

*/