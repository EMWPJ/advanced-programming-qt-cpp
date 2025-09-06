#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001);

    // 动态内存分配示例
    int* ptr = (int*)malloc(sizeof(int));
    *ptr = 100;
    printf("动态分配的值: %d\n", *ptr);
    free(ptr);

    getchar();
    return 0;
}