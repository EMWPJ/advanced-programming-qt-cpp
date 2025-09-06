#include <QHash>
#include <QString>
#include <QDebug>
#include <iostream>

void hashExample()
{
    // 创建QHash并插入元素
    QHash<QString, int> ages;
    ages.insert("Alice", 25);
    ages.insert("Bob", 30);
    ages["Charlie"] = 28;
    ages["David"] = 22;
    qDebug() << "QHash内容:";
    // 遍历QHash（无序）
    QHash<QString, int>::const_iterator it;
    for (it = ages.constBegin(); it != ages.constEnd(); ++it) {
        qDebug() << it.key() << ":" << it.value();
    }
    // 查找元素
    if (ages.contains("Bob")) {
        qDebug() << "Bob的年龄:" << ages.value("Bob");
    }
    // 修改元素
    ages["Alice"] = 26;
    qDebug() << "修改后Alice的年龄:" << ages.value("Alice");
    // 移除元素
    ages.remove("David");
    qDebug() << "移除David后QHash内容:" << ages;
    // 获取所有key和value
    QList<QString> keys = ages.keys();
    QList<int> values = ages.values();
    qDebug() << "所有人名:" << keys;
    qDebug() << "所有年龄:" << values;
    // 清空QHash
    ages.clear();
    qDebug() << "清空后QHash是否为空:" << ages.isEmpty();
}

int main()
{
    hashExample();
    std::cin.get();
    return 0;
}
