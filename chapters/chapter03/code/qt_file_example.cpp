#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileInfo>
#include <QDateTime>
#include <iostream>
void fileExample()
{
    // 1. 创建QFile对象，指定文件名
    QString fileName = "example.txt";
    QFile file(fileName);
    // 2. 以写入模式打开文件（会覆盖原有内容），写入多行内容
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "Hello, QFile!\n";
        out << "Qt文件操作示例。\n";
        out << "当前时间: " << QDateTime::currentDateTime().toString(Qt::ISODate) << "\n";
        out << "支持中文写入。\n";
        file.close();
        qDebug() << "写入文件成功";
    } else {
        qDebug() << "无法打开文件进行写入";
        return;
    }
    // 3. 以追加模式打开文件，追加内容
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << "追加一行内容。\n";
        file.close();
        qDebug() << "追加内容成功";
    } else {
        qDebug() << "无法打开文件进行追加";
    }
    // 4. 以只读模式打开文件，读取全部内容
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        qDebug() << "文件内容：";
        int lineNum = 1;
        while (!in.atEnd()) {
            QString line = in.readLine();
            qDebug() << QString("第%1行: %2").arg(lineNum++).arg(line);
        }
        file.close();
    } else {
        qDebug() << "无法打开文件进行读取";
    }
    // 5. 检查文件是否存在，并获取文件信息
    if (QFile::exists(fileName)) {
        QFileInfo info(fileName);
        qDebug() << fileName << "文件存在";
        qDebug() << "文件大小:" << info.size() << "字节";
        qDebug() << "创建时间:" << info.birthTime().toString(Qt::ISODate);
        qDebug() << "最后修改时间:" << info.lastModified().toString(Qt::ISODate);
        qDebug() << "绝对路径:" << info.absoluteFilePath();
        qDebug() << "是否可读:" << info.isReadable() << "是否可写:" << info.isWritable();
    } else {
        qDebug() << fileName << "文件不存在";
        return;
    }
    // 6. 重命名文件
    QString renamedName = "renamed.txt";
    if (QFile::rename(fileName, renamedName)) {
        qDebug() << "文件重命名成功:" << renamedName;
    } else {
        qDebug() << "文件重命名失败";
        return;
    }
    // 7. 拷贝文件
    QString copyName = "copy.txt";
    if (QFile::copy(renamedName, copyName)) {
        qDebug() << "文件拷贝成功:" << copyName;
    } else {
        qDebug() << "文件拷贝失败";
    }
    // 8. 读取拷贝文件的全部内容为字符串
    QFile copyFile(copyName);
    if (copyFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString allText = copyFile.readAll();
        qDebug() << "copy.txt 全部内容为：\n" << allText;
        copyFile.close();
    }
    // 9. 删除文件
    if (QFile::remove(renamedName)) {
        qDebug() << renamedName << "已删除";
    } else {
        qDebug() << "删除" << renamedName << "失败";
    }
    if (QFile::remove(copyName)) {
        qDebug() << copyName << "已删除";
    } else {
        qDebug() << "删除" << copyName << "失败";
    }
}

int main()
{
    fileExample();
    std::cin.get();
    return 0;
}