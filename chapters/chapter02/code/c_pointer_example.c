#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001);

    // 基本指针操作
    int x = 10;
    int* ptr = &x;

    printf("x的值: %d\n", x);
    printf("x的地址: %p\n", (void*)&x);
    printf("ptr存储的地址: %p\n", (void*)ptr);
    printf("ptr指向的值: %d\n", *ptr);

    // 通过指针修改值
    *ptr = 20;
    printf("修改后x的值: %d\n", x);

    // 指针和数组
    int arr[] = {1, 2, 3, 4, 5};
    int* arrPtr = arr;  // 数组名是指向第一个元素的指针

    printf("通过指针访问数组: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", *(arrPtr + i));
    }
    printf("\n");

    printf("通过指针修改数组: ");
    for (int i = 0; i < 5; i++) {
        *(arrPtr + i) = i * 10;
    }
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }

    // 动态内存分配
    int* dynamicArr = (int*)malloc(5 * sizeof(int));
    if (dynamicArr != NULL) {
        for (int i = 0; i < 5; i++) {
            dynamicArr[i] = i * 10;
        }

        printf("动态数组: ");
        for (int i = 0; i < 5; i++) {
            printf("%d ", dynamicArr[i]);
        }
        printf("\n");

        free(dynamicArr);  // 释放内存
    }

    // 字符串指针
    char* str = "Hello, C!";
    printf("字符串: %s\n", str);
    printf("字符串长度: %zu\n", strlen(str));

    printf("Size of int*: %zu bytes\n", sizeof(int*));
    printf("Size of double*: %zu bytes\n", sizeof(double*));
    printf("Size of char*: %zu bytes\n", sizeof(char*));

    getchar();
    return 0;
}