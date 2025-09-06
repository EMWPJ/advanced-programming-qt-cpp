#include <stdio.h>
#include <windows.h>

// 定义结构体类型
struct Student {
    char name[50]; // 姓名
    int age;       // 年龄
    float score;   // 分数
};

int main() {
    SetConsoleOutputCP(65001);

    // 定义并初始化结构体变量
    struct Student stu = {"张三", 18, 90.5f};

    // 访问结构体成员并输出
    printf("姓名: %s\n", stu.name);
    printf("年龄: %d\n", stu.age);
    printf("分数: %.1f\n", stu.score);

    // 使用结构体指针访问成员
    struct Student *p = &stu;
    printf("通过指针访问姓名: %s\n", p->name);

    getchar();
    return 0;
}