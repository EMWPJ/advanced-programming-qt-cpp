#include <QFileInfo>
#include <QDebug>
#include <QDateTime>
#include <iostream>
void fileInfoExample()
{
    // 1. 构造QFileInfo对象
    QString filePath = "test.txt";
    QFileInfo info(filePath);
    // 2. 判断文件是否存在
    if (info.exists()) {
        qDebug() << "文件存在:" << info.fileName();
    } else {
        qDebug() << "文件不存在:" << filePath;
        return;
    }
    // 3. 获取文件的绝对路径和相对路径
    qDebug() << "绝对路径:" << info.absoluteFilePath();
    qDebug() << "相对路径:" << info.filePath();
    // 4. 获取文件名、后缀、基本名
    qDebug() << "文件名:" << info.fileName();
    qDebug() << "后缀:" << info.suffix();
    qDebug() << "基本名:" << info.baseName();
    // 5. 获取文件大小
    qDebug() << "文件大小(字节):" << info.size();
    // 6. 获取文件的创建、修改、访问时间
    qDebug() << "创建时间:" << info.birthTime().toString();
    qDebug() << "修改时间:" << info.lastModified().toString();
    qDebug() << "访问时间:" << info.lastRead().toString();
    // 7. 判断文件类型
    if (info.isFile()) {
        qDebug() << "这是一个普通文件";
    }
    if (info.isDir()) {
        qDebug() << "这是一个目录";
    }
    // 8. 判断文件权限
    qDebug() << "可读:" << info.isReadable();
    qDebug() << "可写:" << info.isWritable();
    qDebug() << "可执行:" << info.isExecutable();
    // 9. 获取上级目录
    qDebug() << "上级目录:" << info.absolutePath();
}

int main()
{
    fileInfoExample();
    std::cin.get();
    return 0;
}