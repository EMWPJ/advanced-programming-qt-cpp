#include <stdio.h>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001);

    // 变量声明和初始化
    int a = 10;
    int b;
    b = 20;

    // 常量定义
    const int MAX = 100;
    #define PI 3.14159

    // 条件语句
    if (a > b) {
        printf("a is greater than b\n");
    } else {
        printf("b is greater than or equal to a\n");
    }

    // switch语句
    switch (a) {
        case 1:
            printf("a is 1\n");
            break;
        case 2:
            printf("a is 2\n");
            break;
        default:
            printf("a is something else\n");
    }

    // 三元运算符
    int max = (a > b) ? a : b;

    // 逻辑运算符
    if (a > 0 && b > 0) {
        printf("Both a and b are positive\n");
    }

    // for循环
    for (int i = 0; i < 10; i++) {
        printf("%d\n", i);
    }

    // while循环
    int i = 0;
    while (i < 5) {
        printf("%d\n", i);
        i++;
    }

    // do-while循环
    int j = 0;
    do {
        printf("%d\n", j);
        j++;
    } while (j < 3);

    getchar();
    return 0;
}