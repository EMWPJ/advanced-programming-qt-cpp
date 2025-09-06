#include <QQueue>
#include <QString>
#include <QDebug>
#include <iostream>
void queueExample()
{
    // 创建QQueue并入队元素
    QQueue<QString> queue;
    queue.enqueue("任务1");
    queue.enqueue("任务2");
    queue.enqueue("任务3");
    qDebug() << "QQueue内容（从队首到队尾）:";
    for (int i = 0; i < queue.size(); ++i) {
        qDebug() << queue.at(i);
    }

    // 查看队首和队尾元素
    if (!queue.isEmpty()) {
        qDebug() << "队首元素:" << queue.head();
        qDebug() << "队尾元素:" << queue.back();
    }

    // 出队元素
    QString front = queue.dequeue();
    qDebug() << "出队元素:" << front;
    qDebug() << "出队后队首:" << (queue.isEmpty() ? "空" : queue.head());

    // 判断是否为空
    qDebug() << "QQueue是否为空:" << queue.isEmpty();

    // 清空队列
    queue.clear();
    qDebug() << "清空后QQueue是否为空:" << queue.isEmpty();
}

int main()
{
    queueExample();
    std::cin.get();
    return 0;
}
