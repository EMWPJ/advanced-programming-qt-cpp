#include <QObject>
#include <QString>

class MyClass : public QObject
{
    Q_OBJECT  // 启用元对象系统

public:
    explicit MyClass(QObject *parent = nullptr);

    // 属性声明
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int age READ age WRITE setAge NOTIFY ageChanged)

    // 属性访问方法
    QString name() const { return m_name; }
    void setName(const QString &name);

    int age() const { return m_age; }
    void setAge(int age);

signals:
    // 信号声明
    void nameChanged(const QString &name);
    void ageChanged(int age);

private slots:
    // 槽函数声明
    void onNameChanged(const QString &name);

private:
    QString m_name;
    int m_age;
};