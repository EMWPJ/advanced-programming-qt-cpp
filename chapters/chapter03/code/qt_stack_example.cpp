#include <QStack>
#include <QString>
#include <QDebug>
#include <iostream>

void stackExample()
{
    // 创建QStack并压入元素
    QStack<QString> stack;
    stack.push("第一步");
    stack.push("第二步");
    stack.push("第三步");
    qDebug() << "QStack内容（从栈底到栈顶）:";
    for (int i = 0; i < stack.size(); ++i) {
        qDebug() << stack.at(i);
    }

    // 查看栈顶元素
    if (!stack.isEmpty()) {
        qDebug() << "栈顶元素:" << stack.top();
    }

    // 弹出元素
    QString popped = stack.pop();
    qDebug() << "弹出元素:" << popped;
    qDebug() << "弹出后栈顶:" << (stack.isEmpty() ? "空" : stack.top());

    // 判断是否为空
    qDebug() << "QStack是否为空:" << stack.isEmpty();

    // 清空栈
    stack.clear();
    qDebug() << "清空后QStack是否为空:" << stack.isEmpty();
}

int main()
{
    stackExample();
    std::cin.get();
    return 0;
}
