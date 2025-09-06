#include <QApplication>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <iostream>
class MyDialog : public QDialog
{
    Q_OBJECT
public:
    MyDialog(QWidget *parent = nullptr) : QDialog(parent)
    {
        setWindowTitle("QDialog 示例");
        resize(300, 150);
        QVBoxLayout *layout = new QVBoxLayout(this);
        QLabel *label = new QLabel("这是一个自定义 QDialog 对话框", this);
        layout->addWidget(label);
        QPushButton *okBtn = new QPushButton("确定", this);
        QPushButton *cancelBtn = new QPushButton("取消", this);
        layout->addWidget(okBtn);
        layout->addWidget(cancelBtn);
        connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
        connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MyDialog dlg;
    int ret = dlg.exec(); // 模态对话框
    if (ret == QDialog::Accepted) {
        qDebug() << "用户点击了确定";
    } else {
        qDebug() << "用户点击了取消";
    }
    std::cin.get();
    return 0;
}

#include "qt_dialog_example.moc"