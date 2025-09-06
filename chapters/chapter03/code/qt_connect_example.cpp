#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QVBoxLayout>
class SignalSlotExample : public QWidget
{
    Q_OBJECT
public:
    SignalSlotExample(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("Signal Slot Example");
        resize(300, 150);
    }
    void setupConnections() {
        m_button = new QPushButton("Test", this);
        m_label = new QLabel("Status", this);
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(m_label);
        layout->addWidget(m_button);
        setLayout(layout);

        // 方法1：新语法（推荐）
        connect(m_button, &QPushButton::clicked,
                this, &SignalSlotExample::onButtonClicked);

        // 方法2：Lambda表达式
        connect(m_button, &QPushButton::clicked, [this]() {
            qDebug() << "Button clicked via lambda";
        });

        // 方法3：旧语法（不推荐）
        connect(m_button, SIGNAL(clicked()),
                this, SLOT(onButtonClicked()));

        // 方法4：带参数的连接
        connect(this, &SignalSlotExample::statusChanged,
                m_label, &QLabel::setText);
    }

signals:
    void statusChanged(const QString &status);

private slots:
    void onButtonClicked() {
        qDebug() << "Button clicked";
        emit statusChanged("Button was clicked!");
    }
private:
    QPushButton *m_button;
    QLabel *m_label;
};

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    SignalSlotExample w;
    w.setupConnections();
    w.show();
    return app.exec();
}

#include "qt_connect_example.moc"