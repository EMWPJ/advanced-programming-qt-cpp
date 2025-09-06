#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QIcon>
class PushButtonDemo : public QWidget
{
public:
    PushButtonDemo(QWidget *parent = nullptr) : QWidget(parent)
    {
        setWindowTitle("QPushButton 示例");
        resize(480, 180);
        QLabel *label = new QLabel("请选择按钮操作", this);
        label->setAlignment(Qt::AlignCenter);
        QPushButton *btnNormal = new QPushButton("普通按钮", this);
        QPushButton *btnIcon = new QPushButton(QIcon::fromTheme("document-open"), "带图标按钮", this);
        QPushButton *btnCheck = new QPushButton("可切换按钮", this);
        btnCheck->setCheckable(true);
        connect(btnNormal, &QPushButton::clicked, this,
            [=](){ label->setText("点击了普通按钮"); });
        connect(btnIcon, &QPushButton::clicked, this,
            [=](){ label->setText("点击了带图标按钮"); });
        connect(btnCheck, &QPushButton::toggled, this, [=](bool checked){
            label->setText(checked ? "切换按钮：已按下" : "切换按钮：未按下");
        });
        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->addWidget(btnNormal);
        layout->addWidget(btnIcon);
        layout->addWidget(btnCheck);
        layout->addWidget(label, 1);
        setLayout(layout);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    PushButtonDemo w;
    w.show();
    return app.exec();
}
