#include <QCoreApplication>
#include <QThread>
#include <QDebug>
#include <iostream>
// 自定义线程类，继承自QThread
class WorkerThread : public QThread
{
protected:
    void run() override {
        for (int i = 1; i <= 5; ++i) {
            qDebug() << "工作线程正在运行，计数：" << i;
            QThread::sleep(1); // 线程休眠1秒
        }
        qDebug() << "工作线程结束";
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    WorkerThread thread;
    qDebug() << "主线程：启动工作线程";
    thread.start(); // 启动线程，自动调用run()

    // 等待线程结束
    thread.wait();
    qDebug() << "主线程：工作线程已结束";
    app.exec();
    std::cin.get();
    return 0;
}
