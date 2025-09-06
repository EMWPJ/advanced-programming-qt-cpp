#include <QList>
#include <QString>
#include <QDebug>
#include <algorithm>
#include <iostream>

void listExample()
{
    // 创建QList并添加元素
    QList<int> numbers;
    numbers << 10 << 20 << 30;
    numbers.append(40);
    numbers.prepend(5);
    qDebug() << "QList<int>内容:";
    for (int i = 0; i < numbers.size(); ++i) {
        qDebug() << "索引" << i << ":" << numbers.at(i);
    }
    // 插入和移除元素
    numbers.insert(2, 15); // 在索引2插入15
    numbers.removeAt(0);   // 移除第0个元素
    qDebug() << "修改后的QList<int>内容:";
    for (int n : numbers) {
        qDebug() << n;
    }
    // 查找元素
    int idx = numbers.indexOf(30);
    if (idx != -1) {
        qDebug() << "30的索引为:" << idx;
    }
    // 反转和排序
    std::sort(numbers.begin(), numbers.end());
    std::reverse(numbers.begin(), numbers.end());
    qDebug() << "排序并反转后的QList<int>内容:" << numbers;
    // 支持自定义类型
    QList<QString> names;
    names << "Alice" << "Bob" << "Charlie";
    qDebug() << "QList<QString>内容:" << names;
}

int main()
{
    listExample();
    std::cin.get();
    return 0;
}
