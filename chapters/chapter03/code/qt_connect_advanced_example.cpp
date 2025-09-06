#include <QObject>
#include <QDebug>
#include <iostream>
class Receiver : public QObject
{
    Q_OBJECT
public slots:
    void onButtonClicked() {
        qDebug() << "ReceiverA: 按钮被点击（onButtonClicked）";
    }
    void onStatusChanged(const QString &msg) {
        qDebug() << "ReceiverA: 状态变更:" << msg;
    }
};
class Sender : public QObject
{
    Q_OBJECT
public:
    void doSomething() {
        emit statusChanged("Sender: 状态已更新");
    }
signals:
    void statusChanged(const QString &msg);
};

int main()
{
    Receiver* receiver1 = new Receiver();
    Receiver* receiver2 = new Receiver();
    Sender* sender1 = new Sender();
    Sender* sender2 = new Sender();
    // 多对一：多个信号连接同一个槽
    QObject::connect(sender1, &Sender::statusChanged, receiver1, &Receiver::onStatusChanged);
    QObject::connect(sender2, &Sender::statusChanged, receiver1, &Receiver::onStatusChanged);
    sender1->doSomething();
    sender2->doSomething();
    // 断开连接
    QObject::disconnect(sender1, &Sender::statusChanged, receiver1, &Receiver::onStatusChanged);
    QObject::disconnect(sender2, &Sender::statusChanged, receiver1, &Receiver::onStatusChanged);
    sender1->doSomething();
    sender2->doSomething();

    // 一对多：一个信号连接多个槽
    QObject::connect(sender1, &Sender::statusChanged, receiver1, &Receiver::onStatusChanged);
    QObject::connect(sender1, &Sender::statusChanged, receiver2, &Receiver::onStatusChanged);
    sender1->doSomething();
    // 断开全部
    QObject::disconnect(sender1, &Sender::statusChanged, nullptr, nullptr);
    sender1->doSomething();
    // 释放对象时，会自动断开信号和槽的连接
    delete receiver1;
    delete receiver2;
    delete sender1;
    delete sender2;
    std::cin.get();
    return 0;
}

#include "qt_connect_advanced_example.moc"