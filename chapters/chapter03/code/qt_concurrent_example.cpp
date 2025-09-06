#include <QCoreApplication>
#include <QtConcurrent>
#include <QDebug>
#include <QThread>
#include <QVector>
#include <iostream>
// 一个耗时的计算函数
int slowSquare(int x)
{
    QThread::sleep(1); // 模拟耗时操作
    qDebug() << "计算" << x << "的平方，线程ID:" << QThread::currentThreadId();
    return x * x;
}

void concurrentExample()
{
    QVector<int> numbers = {1, 2, 3, 4, 5};
    qDebug() << "主线程ID:" << QThread::currentThreadId();

    // 1. 并行计算每个元素的平方
    QFuture<int> future = QtConcurrent::mapped(numbers, slowSquare);

    // 2. 等待所有任务完成
    future.waitForFinished();

    // 3. 获取结果
    QVector<int> results = future.results().toVector();
    qDebug() << "所有平方结果:" << results;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    concurrentExample();

    std::cin.get();
    return 0;
}
