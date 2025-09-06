#include <QApplication>
#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>

class ListWidgetDemo : public QWidget
{
public:
    ListWidgetDemo(QWidget *parent = nullptr) : QWidget(parent)
    {
        setWindowTitle("QListWidget 示例");
        resize(400, 250);

        auto *label = new QLabel("请选择一个项目", this);
        label->setAlignment(Qt::AlignCenter);

        auto *listWidget = new QListWidget(this);
        listWidget->addItems({"苹果", "香蕉", "橙子", "葡萄", "西瓜"});

        connect(listWidget, &QListWidget::currentTextChanged, this, [=](const QString &text){
            label->setText("已选择：" + text);
        });

        auto *mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(listWidget);
        mainLayout->addWidget(label);
        setLayout(mainLayout);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ListWidgetDemo w;
    w.show();
    return app.exec();
}
