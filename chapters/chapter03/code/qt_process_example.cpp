#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <QStringList>
#include <iostream>
void processExample()
{
    // 1. 创建QProcess对象
    QProcess process;
    // 2. 设置要执行的外部命令（以"ping"为例，跨平台处理）
#if defined(Q_OS_WIN)
    QString program = "ping";
    QStringList arguments;
    arguments << "127.0.0.1" << "-n" << "2";
#else
    QString program = "ping";
    QStringList arguments;
    arguments << "127.0.0.1" << "-c" << "2";
#endif
    // 3. 启动进程并等待完成
    process.start(program, arguments);
    bool finished = process.waitForFinished(5000); // 最多等待5秒
    if (finished) {
        // 4. 读取标准输出
        QString output = process.readAllStandardOutput();
        qDebug() << "进程输出：";
        qDebug().noquote() << output;
        // 5. 读取标准错误
        QString errorOutput = process.readAllStandardError();
        if (!errorOutput.isEmpty()) {
            qDebug() << "进程错误输出：";
            qDebug().noquote() << errorOutput;
        }
        // 6. 获取退出码
        int exitCode = process.exitCode();
        qDebug() << "进程退出码：" << exitCode;
    } else {
        qDebug() << "进程未在规定时间内完成";
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    processExample();
    std::cin.get();
    return 0;
}
