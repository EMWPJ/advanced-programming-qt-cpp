#include <QDir>
#include <QDebug>
#include <QStringList>
#include <iostream>
void dirExample()
{
    // 1. 获取当前工作目录
    QDir currentDir = QDir::current();
    qDebug() << "当前工作目录:" << currentDir.absolutePath();
    // 2. 创建新目录
    QString newDirName = "test_dir";
    if (currentDir.mkdir(newDirName)) {
        qDebug() << "创建目录成功:" << newDirName;
    } else {
        qDebug() << "创建目录失败或已存在:" << newDirName;
    }
    // 3. 进入新目录
    if (currentDir.cd(newDirName)) {
        qDebug() << "进入目录:" << currentDir.absolutePath();
    } else {
        qDebug() << "进入目录失败:" << newDirName;
    }
    // 4. 创建多个文件和子目录
    QFile file1(currentDir.filePath("file1.txt"));
    if (file1.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file1.write("QDir文件1内容\n");
        file1.close();
    }
    QFile file2(currentDir.filePath("file2.txt"));
    if (file2.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file2.write("QDir文件2内容\n");
        file2.close();
    }
    currentDir.mkdir("subdir");
    // 5. 列出当前目录下所有文件和子目录
    qDebug() << "当前目录下的所有文件:";
    QStringList files = currentDir.entryList(QDir::Files);
    for (const QString &file : files) {
        qDebug() << "文件:" << file;
    }
    qDebug() << "当前目录下的所有子目录:";
    QStringList dirs = currentDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &dir : dirs) {
        qDebug() << "子目录:" << dir;
    }
    // 6. 返回上级目录
    currentDir.cdUp();
    qDebug() << "返回上级目录:" << currentDir.absolutePath();
    // 7. 删除文件和目录
    QDir delDir(newDirName);
    delDir.remove("file1.txt");
    delDir.remove("file2.txt");
    delDir.rmdir("subdir");
    if (currentDir.rmdir(newDirName)) {
        qDebug() << "删除目录成功:" << newDirName;
    } else {
        qDebug() << "删除目录失败:" << newDirName;
    }
    // 8. 获取主目录、临时目录、根目录
    qDebug() << "主目录:" << QDir::homePath();
    qDebug() << "临时目录:" << QDir::tempPath();
    qDebug() << "根目录:" << QDir::rootPath();
    // 9. 判断某个路径是否为绝对路径
    QString absPath = "/usr/bin";
    QString relPath = "relative/path";
    qDebug() << absPath << "是否为绝对路径:" << QDir::isAbsolutePath(absPath);
    qDebug() << relPath << "是否为绝对路径:" << QDir::isAbsolutePath(relPath);
    // 10. 拼接路径
    QDir someDir("/home/user");
    QString fullPath = someDir.filePath("myfile.txt");
    qDebug() << "拼接后的完整路径:" << fullPath;
    // 11. 过滤文件（如只列出txt文件）
    QDir filterDir = QDir::current();
    QStringList nameFilters;
    nameFilters << "*.txt";
    QStringList txtFiles = filterDir.entryList(nameFilters, QDir::Files);
    qDebug() << "当前目录下的txt文件:";
    for (const QString &file : txtFiles) {
        qDebug() << file;
    }
    // 12. 遍历目录（递归列出所有文件）
    qDebug() << "递归列出当前目录及子目录下所有文件:";
    QStringList allFiles;
    std::function<void(const QDir&)> listAllFiles = [&](const QDir& dir) {
        QStringList files = dir.entryList(QDir::Files);
        for (const QString &file : files) {
            allFiles << dir.absoluteFilePath(file);
        }
        QStringList subdirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString &subdir : subdirs) {
            QDir nextDir(dir.absoluteFilePath(subdir));
            listAllFiles(nextDir);
        }
    };
    listAllFiles(QDir::current());
    for (const QString &file : allFiles) {
        qDebug() << file;
    }
    // 13. 判断目录是否存在
    QString checkDir = "test_dir";
    if (QDir(checkDir).exists()) {
        qDebug() << checkDir << "目录存在";
    } else {
        qDebug() << checkDir << "目录不存在";
    }
    // 14. 重命名目录
    QString oldName = "old_dir";
    QString newName = "new_dir";
    QDir().mkdir(oldName);
    if (QDir().rename(oldName, newName)) {
        qDebug() << "目录重命名成功:" << oldName << "->" << newName;
        QDir().rmdir(newName);
    } else {
        qDebug() << "目录重命名失败";
    }
    // 15. 计算相对路径
    QString relativePath = QDir::current().relativeFilePath("test_dir");
    qDebug() << "test_dir 相对于当前目录的相对路径:" << relativePath;
}

int main()
{
    dirExample();
    std::cin.get();
    return 0;
}
