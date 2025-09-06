#include "qobject_example.h"
#include <QDebug>
#include <QApplication>
#include <iostream>

MyClass::MyClass(QObject *parent) : QObject(parent), m_age(0) {}

void MyClass::setName(const QString &name) {
    if (m_name != name) {
        m_name = name;
        emit nameChanged(name);
    }
}

void MyClass::setAge(int age) {
    if (m_age != age) {
        m_age = age;
        emit ageChanged(age);
    }
}

void MyClass::onNameChanged(const QString &name) {
    qDebug() << "Name changed to" << name;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MyClass myClass;
    myClass.setName("John");
    myClass.setAge(20);
    app.exec();
    std::cin.get();
    return 0;
}