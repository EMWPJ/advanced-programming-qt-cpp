#include <QApplication>
#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
class CheckBoxDemo : public QWidget
{
public:
    CheckBoxDemo(QWidget *parent = nullptr) : QWidget(parent)
    {
        setWindowTitle("QCheckBox 示例");
        resize(400, 180);
    QLabel *label = new QLabel("请选择复选框", this);
    label->setAlignment(Qt::AlignCenter);
    QCheckBox *check1 = new QCheckBox("选项一", this);
    QCheckBox *check2 = new QCheckBox("选项二", this);
    // 信号槽连接
    auto updateLabel = [=]() {
        QStringList checked;
        if (check1->isChecked()) checked << "选项一";
        if (check2->isChecked()) checked << "选项二";
        if (checked.isEmpty())
            label->setText("未选择任何选项");
        else
            label->setText("已选择：" + checked.join("，"));
    };
    connect(check1, &QCheckBox::toggled, this, updateLabel);
    connect(check2, &QCheckBox::toggled, this, updateLabel);
        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addWidget(check1);
        vbox->addWidget(check2);
        QHBoxLayout *hbox = new QHBoxLayout(this);
        hbox->addLayout(vbox);
        hbox->addWidget(label, 1);
        setLayout(hbox);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    CheckBoxDemo w;
    w.show();
    return app.exec();
}
