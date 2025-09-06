#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <iostream>
void textStreamExample()
{
    // 1. 写入文本文件
    QFile outFile("textstream_test.txt");
    if (outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&outFile);
        out << "第一行文本" << Qt::endl;
        out << "第二行文本" << Qt::endl;
        out << QString("数字: %1").arg(123) << Qt::endl;
        outFile.close();
        qDebug() << "写入文件完成";
    } else {
        qDebug() << "无法打开文件进行写入";
        return;
    }

    // 2. 读取文本文件
    QFile inFile("textstream_test.txt");
    if (inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&inFile);
        qDebug() << "读取文件内容:";
        while (!in.atEnd()) {
            QString line = in.readLine();
            qDebug() << line;
        }
        inFile.close();
    } else {
        qDebug() << "无法打开文件进行读取";
    }
}

int main()
{
    textStreamExample();
    std::cin.get();
    return 0;
}
