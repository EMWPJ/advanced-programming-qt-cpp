#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <QString>
#include <QList>
#include <iostream>
void dataStreamExample()
{
    // 1. 写入二进制数据到文件
    QFile outFile("datastream_test.dat");
    if (outFile.open(QIODevice::WriteOnly)) {
        QDataStream out(&outFile);
        out.setVersion(QDataStream::Qt_5_0);
        int a = 42;
        double b = 3.14159;
        QString str = "Qt数据流测试";
        QList<int> list = {1, 2, 3, 4, 5};
        out << a << b << str << list;
        outFile.close();
        qDebug() << "二进制数据写入完成";
    } else {
        qDebug() << "无法打开文件进行写入";
        return;
    }
    // 2. 从文件读取二进制数据
    QFile inFile("datastream_test.dat");
    if (inFile.open(QIODevice::ReadOnly)) {
        QDataStream in(&inFile);
        in.setVersion(QDataStream::Qt_5_0);
        int a;
        double b;
        QString str;
        QList<int> list;
        in >> a >> b >> str >> list;
        qDebug() << "int:" << a;
        qDebug() << "double:" << b;
        qDebug() << "QString:" << str;
        qDebug() << "QList<int>:" << list;
        inFile.close();
    } else {
        qDebug() << "无法打开文件进行读取";
    }
}

int main()
{
    dataStreamExample();
    std::cin.get();
    return 0;
}
