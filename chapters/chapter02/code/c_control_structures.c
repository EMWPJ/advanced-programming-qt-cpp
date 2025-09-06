// C语言控制结构示例

#include <stdio.h>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001);

    int a = 5;
    int b = 10;

    // if-else语句
    if (a > 0) {
        printf("a是正数\n");
    } else {
        printf("a不是正数\n");
    }

    // 嵌套if语句
    if (a > 0) {
        if (a > 10) {
            printf("a大于10\n");
        } else {
            printf("a在1到10之间\n");
        }
    }

    // switch-case语句
    switch (a) {
        case 1:
            printf("a等于1\n");
            break;
        case 5:
            printf("a等于5\n");
            break;
        default:
            printf("a是其他值\n");
    }

    // 三元运算符
    int max = (a > b) ? a : b;
    printf("最大值是: %d\n", max);

    // for循环
    printf("for循环输出: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", i);
    }
    printf("\n");

    // while循环
    printf("while循环输出: ");
    int i = 0;
    while (i < 3) {
        printf("%d ", i);
        i++;
    }
    printf("\n");

    // do-while循环
    printf("do-while循环输出: ");
    int j = 0;
    do {
        printf("%d ", j);
        j++;
    } while (j < 3);
    printf("\n");

    // break和continue
    printf("break和continue示例: ");
    for (int k = 0; k < 10; k++) {
        if (k == 3) {
            continue;  // 跳过3
        }
        if (k == 7) {
            break;     // 在7处停止
        }
        printf("%d ", k);
    }
    printf("\n");

    getchar();
    return 0;
}