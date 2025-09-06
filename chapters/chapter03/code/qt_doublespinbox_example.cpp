#include <QApplication>
#include <QWidget>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class DoubleSpinBoxDemo : public QWidget
{
public:
    DoubleSpinBoxDemo(QWidget *parent = nullptr) : QWidget(parent)
    {
        setWindowTitle("QDoubleSpinBox 示例");
        resize(400, 180);
        QLabel *label = new QLabel("当前值：0.00", this);
        label->setAlignment(Qt::AlignCenter);
        QDoubleSpinBox *doubleSpinBox = new QDoubleSpinBox(this);
        doubleSpinBox->setRange(-100.0, 100.0);
        doubleSpinBox->setSingleStep(0.5);
        doubleSpinBox->setDecimals(2);
        doubleSpinBox->setValue(0.0);
        doubleSpinBox->setSuffix(" 元");
        // 信号槽连接
        connect(doubleSpinBox,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double value){
            label->setText(QString("当前值：%1").arg(value, 0, 'f', 2));
        });
        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addWidget(doubleSpinBox);
        vbox->addWidget(label, 1);
        setLayout(vbox);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    DoubleSpinBoxDemo w;
    w.show();
    return app.exec();
}
