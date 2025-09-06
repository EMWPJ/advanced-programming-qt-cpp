#include <QStringList>
#include <QString>
#include <QDebug>
#include <iostream>
void stringListExample()
{
    // 1. 创建和初始化QStringList
    QStringList list1;
    list1 << "Apple" << "Banana" << "Orange";
    qDebug() << "初始化的QStringList:" << list1;
    // 2. 通过字符串分割生成QStringList
    QString fruits = QStringLiteral("Mango,Peach,Pear");
    QStringList list2 = fruits.split(',');
    qDebug() << "分割字符串得到的QStringList:" << list2;
    // 3. 拼接QStringList为字符串
    QString joined = list1.join(" | ");
    qDebug() << "用 | 连接的字符串:" << joined;
    // 4. 添加和插入元素
    list1.append("Grape");
    list1.prepend("Pineapple");
    list1.insert(2, "Lemon");
    qDebug() << "添加和插入后的QStringList:" << list1;
    // 5. 查找和移除元素
    int idx = list1.indexOf("Banana");
    if (idx != -1) {
        qDebug() << "\"Banana\"的位置:" << idx;
        list1.removeAt(idx);
    }
    list1.removeAll("Orange");
    qDebug() << "移除元素后的QStringList:" << list1;
    // 6. 判断是否包含某元素
    qDebug() << "是否包含\"Apple\":" << list1.contains("Apple");
    // 7. 遍历QStringList
    qDebug() << "遍历QStringList:";
    for (const QString &fruit : list1) {
        qDebug() << fruit;
    }
    // 8. 过滤和排序
    QStringList filtered = list1.filter("a", Qt::CaseInsensitive);
    qDebug() << "包含字母a的元素:" << filtered;
    list1.sort();
    qDebug() << "排序后的QStringList:" << list1;
    // 9. 转换为QList
    QList<QString> qlist = list1;
    qDebug() << "转换为QList后第一个元素:" << qlist.first();
    // 10. 转换为标准C++容器
    std::vector<QString> stdVec = std::vector<QString>(list1.begin(), list1.end());
    if (!stdVec.empty()) {
        qDebug() << "转换为std::vector后第一个元素:" << stdVec.front();
    }
    // 11. 清空和判断空
    list1.clear();
    qDebug() << "清空后是否为空:" << list1.isEmpty();
}

int main()
{
    stringListExample();
    std::cin.get();
    return 0;
}
