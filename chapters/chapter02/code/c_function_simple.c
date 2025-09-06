#include <stdio.h>
#include <windows.h>

// 函数声明：计算两个整数的和
int add(int a, int b) {
    return a + b; // 返回a和b的和
}

int main() {
    SetConsoleOutputCP(65001);
    // 调用add函数，计算2和3的和
    int result = add(2, 3);
    // 输出结果
    printf("2 + 3 = %d\n", result);
    getchar();
    return 0; // 程序正常结束
}