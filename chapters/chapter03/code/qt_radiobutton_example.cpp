#include <QApplication>
#include <QWidget>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
class RadioButtonDemo : public QWidget
{
public:
    RadioButtonDemo(QWidget *parent = nullptr) : QWidget(parent)
    {
        setWindowTitle("QRadioButton 示例");
        resize(400, 180);
        QLabel *label = new QLabel("请选择一个选项", this);
        label->setAlignment(Qt::AlignCenter);
        QRadioButton *radio1 = new QRadioButton("选项一", this);
        QRadioButton *radio2 = new QRadioButton("选项二", this);
        // 分组，确保单选
        QButtonGroup *group = new QButtonGroup(this);
        group->addButton(radio1, 1);
        group->addButton(radio2, 2);
        // 信号槽连接
connect(radio1, &QRadioButton::toggled, this, [=](bool checked){
        if (checked) label->setText("已选择：选项一");
    });
connect(radio2, &QRadioButton::toggled, this, [=](bool checked){
        if (checked) label->setText("已选择：选项二");
    });
        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addWidget(radio1);
        vbox->addWidget(radio2);
        QHBoxLayout *hbox = new QHBoxLayout(this);
        hbox->addLayout(vbox);
        hbox->addWidget(label, 1);
        setLayout(hbox);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    RadioButtonDemo w;
    w.show();
    return app.exec();
}
