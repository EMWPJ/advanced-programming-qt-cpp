#include <QApplication>
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class ComboBoxDemo : public QWidget
{
public:
    ComboBoxDemo(QWidget *parent = nullptr) : QWidget(parent)
    {
        setWindowTitle("QComboBox 示例");
        resize(400, 180);
        QLabel *label = new QLabel("请选择一个选项", this);
        label->setAlignment(Qt::AlignCenter);
        QComboBox *combo = new QComboBox(this);
        combo->addItem("选项一");
        combo->addItem("选项二");
        combo->addItem("选项三");
        // 信号槽连接
        connect(combo, &QComboBox::currentTextChanged, this, [=](const QString &text){
            label->setText("已选择：" + text);
        });
        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addWidget(combo);
        vbox->addWidget(label, 1);
        setLayout(vbox);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ComboBoxDemo w;
    w.show();
    return app.exec();
}
