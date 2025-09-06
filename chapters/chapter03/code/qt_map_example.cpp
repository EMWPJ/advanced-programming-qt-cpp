#include <QMap>
#include <QString>
#include <QDebug>
#include <iostream>

void mapExample()
{
    // 创建QMap并插入元素
    QMap<QString, int> scores;
    scores.insert("Alice", 90);
    scores.insert("Bob", 85);
    scores["Charlie"] = 92;
    scores["David"] = 88;
    qDebug() << "QMap内容:";
    // 遍历QMap（按key有序）
    QMap<QString, int>::const_iterator it;
    for (it = scores.constBegin(); it != scores.constEnd(); ++it) {
        qDebug() << it.key() << ":" << it.value();
    }
    // 查找元素
    if (scores.contains("Bob")) {
        qDebug() << "Bob的分数:" << scores.value("Bob");
    }
    // 修改元素
    scores["Alice"] = 95;
    qDebug() << "修改后Alice的分数:" << scores.value("Alice");
    // 移除元素
    scores.remove("David");
    qDebug() << "移除David后QMap内容:" << scores;
    // 获取所有key和value
    QList<QString> keys = scores.keys();
    QList<int> values = scores.values();
    qDebug() << "所有学生:" << keys;
    qDebug() << "所有分数:" << values;
    // 清空QMap
    scores.clear();
    qDebug() << "清空后QMap是否为空:" << scores.isEmpty();
}

int main()
{
    mapExample();
    std::cin.get();
    return 0;
}
