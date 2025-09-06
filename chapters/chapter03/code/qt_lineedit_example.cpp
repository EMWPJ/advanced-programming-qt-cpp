#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
class LineEditDemo : public QWidget
{
public:
LineEditDemo(QWidget *parent = nullptr) : QWidget(parent)
    {
        setWindowTitle("QLineEdit 示例");
        resize(400, 180);
        QLabel *label = new QLabel("请输入内容", this);
        label->setAlignment(Qt::AlignCenter);
        QLineEdit *lineEdit1 = new QLineEdit(this);
        lineEdit1->setPlaceholderText("普通输入框");
        QLineEdit *lineEdit2 = new QLineEdit(this);
        lineEdit2->setPlaceholderText("只读输入框");
        lineEdit2->setText("只读内容");
        lineEdit2->setReadOnly(true);
        QLineEdit *lineEdit3 = new QLineEdit(this);
        lineEdit3->setPlaceholderText("密码输入框");
        lineEdit3->setEchoMode(QLineEdit::Password);
        // 信号槽连接
        connect(lineEdit1, &QLineEdit::textChanged,
        this, [=](const QString &text){
        label->setText("输入内容：" + text); });
        connect(lineEdit3, &QLineEdit::returnPressed,
        this, [=](){label->setText("密码已输入"); });
        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addWidget(lineEdit1);
        vbox->addWidget(lineEdit2);
        vbox->addWidget(lineEdit3);
        vbox->addWidget(label, 1);
        setLayout(vbox);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    LineEditDemo w;
    w.show();
    return app.exec();
}
