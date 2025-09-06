#include <QByteArray>
#include <QList>
#include <QDebug>
#include <vector>
#include <string>
#include <iostream>
void byteArrayExample()
{
    // 1. 多种方式创建QByteArray
    QByteArray ba1("Hello, Qt!"); // C字符串初始化
    QByteArray ba2 = QByteArray::fromStdString(std::string("QByteArray示例")); // std::string初始化
    QByteArray ba3(10, 'A'); // 10个'A'
    QByteArray ba4; // 空QByteArray
    qDebug() << "ba1原始内容:" << ba1;
    qDebug() << "ba2原始内容:" << ba2;
    qDebug() << "ba3原始内容:" << ba3;
    qDebug() << "ba4是否为空:" << ba4.isEmpty();
    // 2. 追加、插入、前置、替换、移除
    ba1.append(" Welcome.");
    ba1.prepend(">>> ");
    ba1.insert(7, "C++ ");
    qDebug() << "追加、前置、插入后:" << ba1;
    ba1.replace("C++", "Qt/C++");
    ba1.remove(0, 4);
    qDebug() << "替换和移除后:" << ba1;
    // 3. 查找、判断、访问
    int pos = ba1.indexOf("Qt");
    if (pos != -1) {
        qDebug() << "“Qt”在ba1中的位置:" << pos;
    }
    qDebug() << "ba1是否包含Welcome:" << ba1.contains("Welcome");
    qDebug() << "ba1的长度:" << ba1.length();
    if (!ba1.isEmpty())
        qDebug() << "ba1的第一个字符:" << ba1.at(0);
    // 4. 分割、拼接、遍历
    QByteArray ba5("apple,banana,orange,pear");
    QList<QByteArray> list = ba5.split(',');
    qDebug() << "分割结果:";
    int idx = 0;
    for (const QByteArray &item : list) {
        qDebug() << QString("第%1个元素: %2").arg(idx++).arg(QString::fromUtf8(item));
    }
    QByteArray joined = list.join(" | ");
    qDebug() << "用 | 拼接结果:" << joined;
    // 5. 转换为数字、十六进制、Base64、URL编码
    QByteArray ba6("12345");
    int num = ba6.toInt();
    double dnum = QByteArray("3.14159").toDouble();
    qDebug() << "字符串转整数:" << num;
    qDebug() << "字符串转浮点数:" << dnum;
    QByteArray hex = ba6.toHex();
    qDebug() << "十六进制编码:" << hex;
    QByteArray base64 = ba6.toBase64();
    qDebug() << "Base64编码:" << base64;
    QByteArray urlEncoded = ba6.toPercentEncoding();
    qDebug() << "URL编码:" << urlEncoded;
    // 6. 解码
    QByteArray fromHex = QByteArray::fromHex(hex);
    QByteArray fromBase64 = QByteArray::fromBase64(base64);
    QByteArray fromUrl = QByteArray::fromPercentEncoding(urlEncoded);
    qDebug() << "十六进制解码:" << fromHex;
    qDebug() << "Base64解码:" << fromBase64;
    qDebug() << "URL解码:" << fromUrl;
    // 7. 与QString互转
    QString str1 = QString::fromUtf8(ba1);
    QByteArray ba7 = str1.toUtf8();
    qDebug() << "QByteArray转QString:" << str1;
    qDebug() << "QString转QByteArray:" << ba7;
    // 8. 直接访问底层数据、修改内容
    const char *raw = ba1.constData();
    char *data = ba1.data();
    qDebug() << "底层数据(只读):" << raw;
    if (ba1.size() > 0) {
        data[0] = 'q';
        qDebug() << "修改后ba1:" << ba1;
    }
    // 9. 截取、清空、判断空
    QByteArray sub = ba1.mid(2, 5);
    qDebug() << "mid截取:" << sub;
    QByteArray left = ba1.left(4);
    QByteArray right = ba1.right(6);
    qDebug() << "left截取:" << left;
    qDebug() << "right截取:" << right;
    ba1.clear();
    qDebug() << "清空后ba1是否为空:" << ba1.isEmpty();
    // 10. QByteArray与std::string、std::vector<char>互转
    std::string stdStr = ba6.toStdString();
    QByteArray ba8 = QByteArray::fromStdString(stdStr);
    qDebug() << "QByteArray转std::string:" << QString::fromStdString(stdStr);
    qDebug() << "std::string转QByteArray:" << ba8;
    std::vector<char> vec(ba5.begin(), ba5.end());
    QByteArray ba9(vec.data(), static_cast<int>(vec.size()));
    qDebug() << "QByteArray转std::vector<char>:" << QByteArray(vec.data(), static_cast<int>(vec.size()));
    qDebug() << "std::vector<char>转QByteArray:" << ba9;
}

int main()
{
    byteArrayExample();
    std::cin.get();
    return 0;
}
