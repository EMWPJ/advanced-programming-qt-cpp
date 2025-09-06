#include <QApplication>
#include <QWidget>
#include <QSpinBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class SpinBoxDemo : public QWidget
{
public:
    SpinBoxDemo(QWidget *parent = nullptr) : QWidget(parent)
    {
        setWindowTitle("QSpinBox 示例");
        resize(400, 180);
        QLabel *label = new QLabel("当前值：0", this);
        label->setAlignment(Qt::AlignCenter);
        QSpinBox *spinBox = new QSpinBox(this);
        spinBox->setRange(0, 100);
        spinBox->setSingleStep(5);
        spinBox->setValue(0);
        spinBox->setSuffix(" 个");
        // 信号槽连接
        connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value){
            label->setText(QString("当前值：%1").arg(value));
        });
        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addWidget(spinBox);
        vbox->addWidget(label, 1);
        setLayout(vbox);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SpinBoxDemo w;
    w.show();
    return app.exec();
}
