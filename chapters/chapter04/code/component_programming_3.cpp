#include <QCoreApplication>
#include <QObject>
#include <QString>
#include <QDebug>
#include <QList>
#include <iostream>
// Qt组件基类（QObject派生）
class MyComponent : public QObject {
    Q_OBJECT
public:
    MyComponent(const QString& name, QObject* parent = nullptr)
        : QObject(parent), m_name(name) {}
    QString name() const { return m_name; }
public slots:
    void process() { qDebug() << m_name << "正在处理任务"; }
private:
    QString m_name;
};
// 组件管理器，负责管理和组合多个Qt组件
class QtComponentManager : public QObject {
    Q_OBJECT
public:
    void addComponent(MyComponent* comp) { m_components.append(comp); }
    void processAll() {
        for (auto comp : m_components) {
            comp->process();
        }
    }
private:
    QList<MyComponent*> m_components;
};
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    MyComponent compA("QtComponentA");
    MyComponent compB("QtComponentB");
    QtComponentManager manager;
    manager.addComponent(&compA);
    manager.addComponent(&compB);
    manager.processAll();
    std::cin.get();
    return 0;
}

#include "component_programming_3.moc"
