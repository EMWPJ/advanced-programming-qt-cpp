#include <QVariant>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QByteArray>
#include <QList>
#include <QMap>
#include <QDebug>
#include <iostream>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 7, 0))
#include <QJsonObject>
#endif
// 自定义类型声明与注册（建议放在全局，避免多次注册）
struct MyType {
    int x;
};
Q_DECLARE_METATYPE(MyType)

void variantExample()
{
    // 1. 存储基本类型
    QVariant vInt = 42;
    QVariant vDouble = 3.14159;
    QVariant vBool = true;
    QVariant vString = QStringLiteral("Hello QVariant");
    QVariant vDateTime = QDateTime::currentDateTime();
    qDebug() << "int:" << vInt.toInt()
             << "double:" << vDouble.toDouble()
             << "bool:" << vBool.toBool()
             << "QString:" << vString.toString()
             << "QDateTime:" << vDateTime.toDateTime().toString(Qt::ISODate);
    // 2. 存储Qt容器类型
    QStringList strList = {"Apple", "Banana", "Orange"};
    QVariant vStrList = strList;
    qDebug() << "QStringList:" << vStrList.toStringList();
    QList<int> intList = {1, 2, 3, 4, 5};
    QVariant vIntList = QVariant::fromValue(intList);
    qDebug() << "QList<int>:" << vIntList.value<QList<int>>();
    // 3. 存储QByteArray
    QByteArray ba("Qt Variant");
    QVariant vByteArray = ba;
    qDebug() << "QByteArray:" << vByteArray.toByteArray();
    // 4. 类型判断与转换
    if (vInt.typeId() == QMetaType::Int)
        qDebug() << "vInt是int类型";
    if (vString.canConvert<QString>())
        qDebug() << "vString可以转换为QString:" << vString.toString();
    // 5. QVariant的空值和有效性判断
    QVariant vNull;
    qDebug() << "vNull是否有效:" << vNull.isValid()
             << "vNull是否为null:" << vNull.isNull();
    // 6. QVariant的类型安全访问
    vInt.setValue(2024);
    qDebug() << "类型安全访问int:" << vInt.value<int>();
    // 7. QVariant的拷贝与比较
    QVariant vCopy = vString;
    qDebug() << "vCopy == vString:" << (vCopy == vString);
    vCopy = 100;
    qDebug() << "vCopy赋新值后:" << vCopy;
    // 8. QVariant的类型名与类型ID
    qDebug() << "vInt类型名:" << vInt.typeName()
             << "vString类型ID:" << vString.typeId();
    // 9. QVariant与QMap、QList嵌套使用
    QMap<QString, QVariant> map{
        {"name", "Tom"},
        {"age", 18},
        {"score", 95.5}
    };
    qDebug() << "QMap<QString, QVariant>内容:";
    for (auto it = map.cbegin(); it != map.cend(); ++it)
        qDebug() << it.key() << ":" << it.value();
    QList<QVariant> varList = {1, 2.5, "abc", QDateTime::currentDateTime()};
    qDebug() << "QList<QVariant>内容:";
    for (const QVariant &v : varList)
        qDebug() << v;
    // 10. QVariant的toJsonValue、toJsonObject、toJsonArray（Qt 5.7+）
#if (QT_VERSION >= QT_VERSION_CHECK(5, 7, 0))
    QVariantMap jsonMap{{"user", "Alice"}, {"id", 123}};
    QJsonObject obj = QJsonObject::fromVariantMap(jsonMap);
    qDebug() << "QVariantMap转QJsonObject:" << obj;
#endif
    // 11. QVariant的swap
    QVariant a = 1, b = 2;
    a.swap(b);
    qDebug() << "swap后a:" << a << "b:" << b;
    // 12. QVariant的clear
    a.clear();
    qDebug() << "clear后a是否有效:" << a.isValid();
    // 13. QVariant的toStringList、toList、toMap等
    QVariant vList = QVariant::fromValue(QList<QVariant>{1, 2, 3});
    qDebug() << "QVariant转QList<QVariant>:" << vList.toList();
    QVariant vMap = QVariant::fromValue(QVariantMap{{"k1", 1}, {"k2", 2}});
    qDebug() << "QVariant转QVariantMap:" << vMap.toMap();
    // 14. QVariant的isNull/isValid区别演示
    QVariant vEmptyString = QString();
    qDebug() << "空QString的QVariant isNull:" << vEmptyString.isNull()
             << "isValid:" << vEmptyString.isValid();
    // 15. QVariant的setValue与模板value<T>()的配合
    QVariant vAny;
    vAny.setValue(QString("AnyType"));
    qDebug() << "setValue后value<QString>:" << vAny.value<QString>();
    // 16. QVariant与自定义类型（注册自定义类型）
    // 注册类型（通常在main函数或程序初始化时注册一次）
    static bool registered = [](){
        qRegisterMetaType<MyType>("MyType");
        return true;
    }();
    MyType myObj{10};
    QVariant vCustom = QVariant::fromValue(myObj);
    if (vCustom.canConvert<MyType>()) {
        MyType outObj = vCustom.value<MyType>();
        qDebug() << "自定义类型MyType.x:" << outObj.x;
    }
}

int main()
{
    variantExample();
    std::cin.get();
    return 0;
}
