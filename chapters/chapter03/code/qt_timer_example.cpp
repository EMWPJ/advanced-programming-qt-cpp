#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include <iostream>
// 定时器回调槽函数
void onTimeout()
{
    static int count = 0;
    count++;
    qDebug() << "定时器超时，第" << count << "次";
    if (count >= 5) {
        // 超过5次后退出应用
        QCoreApplication::quit();
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // 1. 创建QTimer对象
    QTimer timer;
    // 2. 连接timeout信号到槽函数
    QObject::connect(&timer, &QTimer::timeout, &onTimeout);
    // 3. 设置定时器间隔（每隔1秒触发一次）
    timer.setInterval(1000);
    // 4. 启动定时器
    timer.start();

    qDebug() << "定时器已启动，每秒触发一次，共5次后退出";
    app.exec();
    std::cin.get();
    return 0;
}