#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// 函数声明（原型）
int add(int a, int b);                  // 值传递
void swap(int *a, int *b);              // 引用传递（通过指针）
void printArray(int arr[], int size);   // 值传递（数组名作为指针传递）
int* createArray(int size);

// 函数定义
// 值传递示例：add函数
int add(int a, int b) {
    return a + b;
}

// 引用传递示例：swap函数
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 打印数组
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// 创建动态数组
int* createArray(int size) {
    int* arr = (int*)malloc(size * sizeof(int));
    if (arr != NULL) {
        for (int i = 0; i < size; i++) {
            arr[i] = i + 1;
        }
    }
    return arr;
}

int main() {
    SetConsoleOutputCP(65001);

    // 函数调用——值传递
    int result = add(5, 3);
    printf("5 + 3 = %d\n", result);

    // 函数调用——引用传递
    int x = 10, y = 20;
    printf("交换前: x = %d, y = %d\n", x, y);
    swap(&x, &y); // 传递变量地址，实现引用传递
    printf("交换后: x = %d, y = %d\n", x, y);

    // 数组操作
    int numbers[] = {1, 2, 3, 4, 5};
    printArray(numbers, 5);

    // 动态数组
    int* dynamicNumbers = createArray(5);
    if (dynamicNumbers != NULL) {
        printArray(dynamicNumbers, 5);
        free(dynamicNumbers);
    }

    getchar();
    return 0;
}