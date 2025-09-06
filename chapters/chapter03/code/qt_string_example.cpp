#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QDebug>
#include <iostream>
void stringExample()
{
    // 1. 创建和初始化QString
    QString str1 = QStringLiteral("Hello");
    QString str2 = QStringLiteral("世界");
    QString str3 = QString::fromUtf8("Qt字符串示例");
    // 2. 拼接字符串
    QString str4 = QString("%1, %2!").arg(str1, str2); // 推荐使用arg格式化
    str4.append(QStringLiteral(" 欢迎使用Qt."));
    qDebug() << "拼接后的字符串:" << str4;
    // 3. 格式化字符串

    int year = 2024;
    double pi = 3.14159;
    QString str5 = QStringLiteral("今年是%1, 圆周率约为%2").arg(year).arg(pi, 0, 'f', 2);
    qDebug() << "格式化字符串:" << str5;
    // 4. 查找和替换
    int pos = str4.indexOf(QStringLiteral("Qt"));
    if (pos != -1) {
        qDebug() << R"("Qt"在str4中的位置:)" << pos;
    }
    QString str6 = str4;
    str6.replace(QStringLiteral("Qt"), QStringLiteral("C++"));
    qDebug() << "替换后的字符串:" << str6;
    // 5. 分割和连接
    QString fruits = QStringLiteral("Apple,Banana,Orange");
    QStringList fruitList = fruits.split(',');
    qDebug() << "分割后的QStringList:" << fruitList;
    QString joined = fruitList.join(QStringLiteral(" | "));
    qDebug() << "用 | 连接后的字符串:" << joined;

    // 6. 大小写转换
    qDebug() << "大写:" << str1.toUpper();
    qDebug() << "小写:" << str2.toLower();
    // 7. 去除空白
    QString str7 = QStringLiteral("   Qt is fun!   ");
    qDebug() << "去除前后空白:" << str7.trimmed();
    // 8. 转换为标准C++字符串
    std::string stdStr = str1.toStdString();
    qDebug() << "转换为std::string:" << QString::fromStdString(stdStr);
    // 9. 转换为数字
    QString numStr = QStringLiteral("12345");
    int num = numStr.toInt();
    qDebug() << "QString转int:" << num;
    // 10. 判断是否为空
    QString emptyStr;
    qDebug() << "emptyStr是否为空:" << emptyStr.isEmpty();
    // 11. 字符串长度
    qDebug() << "str4的长度:" << str4.length();
    qDebug() << "str4的大小:" << str4.size();

    // 12. 字符串截取
    QString str8 = QStringLiteral("Hello, Qt World!");
    qDebug() << "mid截取:" << str8.mid(7, 2);   // 从索引7开始，取2个字符
    qDebug() << "left截取:" << str8.left(5);     // 左边5个字符
    qDebug() << "right截取:" << str8.right(6);   // 右边6个字符
    // 13. 判断字符串内容
    qDebug() << "str8是否包含'Qt':" << str8.contains(QStringLiteral("Qt"));
    qDebug() << "str8是否以'Hello'开头:" << str8.startsWith(QStringLiteral("Hello"));
    qDebug() << "str8是否以'World!'结尾:" << str8.endsWith(QStringLiteral("World!"));
    // 14. 字符串比较
    QString cmp1 = QStringLiteral("abc");
    QString cmp2 = QStringLiteral("Abc");
    qDebug() << "区分大小写比较:" << (cmp1 == cmp2);
    qDebug() << "不区分大小写比较:" << (cmp1.compare(cmp2, Qt::CaseInsensitive) == 0);

    // 15. 字符串插入和移除
    QString str9 = QStringLiteral("I Qt!");
    str9.insert(2, QStringLiteral("love "));
    qDebug() << "插入后的字符串:" << str9;
    str9.remove(2, 5); // 从索引2移除5个字符
    qDebug() << "移除后的字符串:" << str9;
    // 16. 重复字符串
    QString repeated = QStringLiteral("Ha").repeated(3);
    qDebug() << "重复字符串:" << repeated;
    // 17. 转换为数字失败的处理
    QString invalidNum = QStringLiteral("abc");
    bool ok = false;
    int invalid = invalidNum.toInt(&ok);
    qDebug() << "转换非法数字字符串, ok:" << ok << ", value:" << invalid;

    // 18. 字符串与QByteArray互转
    QString unicodeStr = QStringLiteral("你好, Qt!");
    QByteArray ba = unicodeStr.toUtf8();
    qDebug() << "转为QByteArray:" << ba;
    QString fromBa = QString::fromUtf8(ba);
    qDebug() << "QByteArray转回QString:" << fromBa;
    // 19. 字符串格式化（静态方法）
    QString formatted = QStringLiteral("%1 + %2 = %3").arg(2).arg(3).arg(2+3);
    qDebug() << "静态格式化:" << formatted;
    // 20. 遍历字符串中的字符
    QString str10 = QStringLiteral("Qt");
    for (const QChar &ch : str10) {
        qDebug() << "字符:" << ch;
    }
}

int main()
{
    stringExample();
    std::cin.get();
    return 0;
}
