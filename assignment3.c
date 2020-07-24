#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include <time.h>
#include <windows.h>

int sorted[1000];
int buckets[1000][1000];
int buckets_size[1000] = { 0 };
int temp[1000];
int newarr[1000];

__int64 GetMicroSecond()
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER now;

    if (!QueryPerformanceFrequency(&frequency))
        return (__int64)GetTickCount();

    if (!QueryPerformanceCounter(&now))
        return (__int64)GetTickCount();

    return ((now.QuadPart) / (frequency.QuadPart / 1000000));
}

void swap(int *x, int *y) {    
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

void merge(int arr[], int left, int mid, int right) {
    int i, j, k, l;
    i = left;
    j = mid + 1;
    k = left;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j])
            sorted[k++] = arr[i++];
        else
            sorted[k++] = arr[j++];
    }

    if (i > mid) {
        for (l = j; l <= right; l++)
            sorted[k++] = arr[l];
    }
    
    else {
        for (l = i; l <= mid; l++)
            sorted[k++] = arr[l];
    }

    for (l = left; l <= right; l++) {
        arr[l] = sorted[l];
    }
}

// 3. mergeSort
void mergeSort(int arr[], int left, int right) {
    int mid;

    if (left < right) {
        mid = (left + right) / 2;
        mergeSort(arr, left, mid); 
        mergeSort(arr, mid + 1, right); 
        merge(arr, left, mid, right);
    }
}

int partition(int arr[], int left, int right) {
    int pivot, temp;
    int low, high;

    low = left;
    high = right + 1;
    pivot = arr[left]; 
  
    while (low < high) {
        while (low < right && arr[low] < pivot) {
            low++;
        }
        while (high > left && arr[high] > pivot) {
            high--;
        }
        if (low < high) {
            swap(&arr[low], &arr[high]);
        }
    }
    
    swap(&arr[left], &arr[high]);

    
    return high;    // same as low
}

// 4. quickSort
void quickSort(int arr[], int left, int right) {
    if (left < right) {
        int sorted = partition(arr, left, right);  

        quickSort(arr, left, sorted - 1);
        quickSort(arr, sorted + 1, right);
    }
}

// 1. bubbleSort
void bubbleSort(int arr[], int size) {
    int i, j;
   
    for (i = 0; i < size - 1; i++) {
        for (j = 0; j < size - 1; j++) {
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
        }
    }
}

// 2. insertionSort
void insertionSort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}


int findMax(int arr[], int n) {     // 배열에서 최대값을 찾아주는 함수
    int max = arr[0];
    for (int i = 0; i < n; i++) {
        if (arr[i] > max)
            max = arr[i];
    }
    return max;
}

void countingDigitSort(int arr[], int n, int div) {
    int Aux = findMax(arr, n);
    int* c = (int*)malloc(sizeof(int) * (Aux + 1));


    for (int i = 0; i < n; i++) {
        newarr[i] = arr[i] % div;
    } 

    for (int i = 0; i <= Aux; i++) {     // 0 으로 초기화
        c[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        c[newarr[i]] = c[newarr[i]]++;
    }

    for (int i = 1; i <= Aux; i++) {
        c[i] = c[i] + c[i - 1];
    }

    for (int i = n; i >= 1; i--) {
        temp[c[newarr[i-1]]] = arr[i-1];
        c[newarr[i-1]]--;
    }

    for (int i = 0; i < n; i++) {
        arr[i] = temp[i+1];
    }

    free(c);
}


void radixSort(int arr[], int n) {
    int k = findMax(arr, n);
    int div = 10;
   
    while (k >= 1) {
        countingDigitSort(arr, n, div);
        div *= 10;
        k /= 10;
    }
  
}

void bucketSort(int arr[], int n) {
    int i, j, index = 0;
    int max = findMax(arr, n);
    int size = 0;

    switch (n) {    // arr[]의 크기에 따라 bucket 수 정의
    case 10:
        size = 5;
        break;
    case 100:
        size = 10;
        break;
    case 1000:
        size = 20;
        break;
    }

    for (i = 0; i < n; i++) {   // arr[i]의 값에 따라 maxValue를 기준으로 범위를 설정한 buckets에 저장
        buckets[((size-1) * arr[i]) / max][buckets_size[((size - 1) * arr[i]) / max]++] = arr[i];
    }
    
    for (i = 0; i < size; i++) {
        if (buckets_size[i]) {     // if bucket is not empty
            insertionSort(buckets[i], buckets_size[i]);     // bucket 내 정렬 insertionSort 사용
        }
        for (j = 0; j < buckets_size[i]; j++) {
            arr[index++] = buckets[i][j];
            buckets[i][j] = 0;
        }
        buckets_size[i] = 0;        // bucket_size 0으로 초기화
    }
   
}

int main(void) {
    
    int select, size;

    __int64 tStart = 0;

    for (int i = 0; i < 1000; i++) {
        buckets_size[i] = 0;
        for (int j = 0; j < 1000; j++) {
            buckets[i][j] = 0;
        }
    }

    while (1) {

        srand((unsigned)time(NULL));

        printf("----Input Size-----> ");
        scanf("%d", &size);
        int* inputData1 = (int*)malloc(sizeof(int) * size + 1);

        

        printf("<---Select Sort--->\n");
        printf("<1. BubbleSort    >\n");
        printf("<2. InsertionSort >\n");
        printf("<3. MergeSort     >\n");
        printf("<4  QuickSort     >\n");
        printf("<5. RadixSort     >\n");
        printf("<6. BucketSort    >\n");
        printf("Input number------> ");

        scanf("%d", &select);

        for (int i = 0; i < size; i++) {
            *(inputData1 + i) = size - i - 1;
        }

        printf("<inputData1 Array>\n");
        for (int i = 0; i < size; i++) {
            printf("%d ", inputData1[i]);
        }
       
            switch (select) {
            case 1:
                tStart = GetMicroSecond();
                bubbleSort(inputData1, size);
                printf("\nelapsed time : %I64d microsec\n", GetMicroSecond() - tStart);
                printf("\n<bubbleSorted Array>\n");

                break;
            case 2:
                tStart = GetMicroSecond();
                insertionSort(inputData1, size);
                printf("\nelapsed time : %I64d microsec\n", GetMicroSecond() - tStart);
                printf("\n<insertionSorted Array>\n");
                break;
            case 3:
                tStart = GetMicroSecond();
                mergeSort(inputData1, 0, size - 1);
                printf("\nelapsed time : %I64d microsec\n", GetMicroSecond() - tStart);
                printf("\n<mergeSorted Array>\n");
                break;
            case 4:
                tStart = GetMicroSecond();
                quickSort(inputData1, 0, size - 1);
                printf("\nelapsed time : %I64d microsec\n", GetMicroSecond() - tStart);
                printf("\n<quickSorted Array>\n");
                break;
            case 5:
                tStart = GetMicroSecond();
                radixSort(inputData1, size);
                printf("\nelapsed time : %I64d microsec\n", GetMicroSecond() - tStart);
                printf("\n<radixSorted Array>\n");
                break;
            case 6:
                tStart = GetMicroSecond();
                bucketSort(inputData1, size);
                printf("\nelapsed time : %I64d microsec\n", GetMicroSecond() - tStart);
                printf("\n<bucketSorted Array>\n");
                for (int i = 0; i < 1000; i++) {
                    buckets_size[i] = 0;
                    for (int j = 0; j < 1000; j++) {
                        buckets[i][j] = 0;
                    }
                }
                break;
            default:
                break;
            }
       
        
        for (int i = 0; i < size; i++) {        // sorted array
            printf("%d ", inputData1[i]);
        }
        
        printf("\n");
        free(inputData1);
    }
    
	
	system("pause");
	return 0;
}