#include <QCoreApplication>
#include <QSettings>
#include <QDebug>
#include <QString>
#include <QVariant>
#include <iostream>
void settingsExample()
{
    // 1. 创建QSettings对象（以INI文件格式，指定组织和应用名）
    QSettings settings("MyCompany", "MyApp");
    // 2. 写入各种类型的配置项
    settings.setValue("username", "alice");
    settings.setValue("window/width", 800);
    settings.setValue("window/height", 600);
    settings.setValue("volume", 0.75);
    settings.setValue("isAdmin", true);
    // 3. 分组写入
    settings.beginGroup("network");
    settings.setValue("host", "192.168.1.100");
    settings.setValue("port", 8080);
    settings.endGroup();
    // 4. 读取配置项（带默认值）
    QString username = settings.value("username", "defaultuser").toString();
    int width = settings.value("window/width", 1024).toInt();
    int height = settings.value("window/height", 768).toInt();
    double volume = settings.value("volume", 1.0).toDouble();
    bool isAdmin = settings.value("isAdmin", false).toBool();
    qDebug() << "用户名:" << username;
    qDebug() << "窗口宽度:" << width;
    qDebug() << "窗口高度:" << height;
    qDebug() << "音量:" << volume;
    qDebug() << "是否管理员:" << isAdmin;
    // 5. 读取分组内的配置
    settings.beginGroup("network");
    QString host = settings.value("host", "localhost").toString();
    int port = settings.value("port", 80).toInt();
    settings.endGroup();
    qDebug() << "网络主机:" << host;
    qDebug() << "网络端口:" << port;
    // 6. 判断配置项是否存在
    if (settings.contains("username")) {
        qDebug() << "存在用户名配置";
    }
    // 7. 删除配置项
    settings.remove("isAdmin");
    qDebug() << "删除isAdmin后，isAdmin存在吗？" << settings.contains("isAdmin");
    // 8. 枚举所有键
    QStringList keys = settings.allKeys();
    qDebug() << "所有配置项键值：";
    for (const QString &key : keys) {
        qDebug() << key << ":" << settings.value(key).toString();
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    settingsExample();
    std::cin.get();
    return 0;
}
