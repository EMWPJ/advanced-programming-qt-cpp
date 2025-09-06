#include <QSet>
#include <QString>
#include <QDebug>
#include <iostream>
void setExample()
{
    // 创建QSet并添加元素
    QSet<QString> fruits;
    fruits << "Apple" << "Banana" << "Orange";
    fruits.insert("Grape");
    fruits.insert("Banana"); // 重复元素不会被添加
    qDebug() << "QSet内容:";
    for (const QString &fruit : fruits) {
        qDebug() << fruit;
    }
    // 判断元素是否存在
    if (fruits.contains("Apple")) {
        qDebug() << "集合中包含Apple";
    }
    // 移除元素
    fruits.remove("Orange");
    qDebug() << "移除Orange后QSet内容:" << fruits;
    // 集合操作：并集、交集、差集
    QSet<QString> tropicalFruits;
    tropicalFruits << "Banana" << "Mango" << "Pineapple";
    QSet<QString> unionSet = fruits.unite(tropicalFruits);      // 并集
    QSet<QString> intersectSet = fruits.intersect(tropicalFruits); // 交集
    QSet<QString> diffSet = fruits.subtract(tropicalFruits);     // 差集
    qDebug() << "并集:" << unionSet;
    qDebug() << "交集:" << intersectSet;
    qDebug() << "差集:" << diffSet;
    // 清空集合
    fruits.clear();
    qDebug() << "清空后QSet是否为空:" << fruits.isEmpty();
}

int main()
{
    setExample();
    std::cin.get();
    return 0;
}
