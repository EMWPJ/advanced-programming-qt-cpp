#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class LabelDemo : public QWidget
{
public:
    LabelDemo(QWidget *parent = nullptr) : QWidget(parent)
    {
        setWindowTitle("QLabel 示例");
        resize(400, 250);

        QLabel *label1 = new QLabel("普通文本标签", this);
        label1->setAlignment(Qt::AlignCenter);

        QLabel *label2 = new QLabel("<font color='blue'>富文本标签</font>", this);
        label2->setAlignment(Qt::AlignCenter);

        QLabel *label3 = new QLabel("<a href='https://www.qt.io/'>Qt 官网</a>", this);
        label3->setAlignment(Qt::AlignCenter);
        label3->setOpenExternalLinks(true);

        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addWidget(label1);
        vbox->addWidget(label2);
        vbox->addWidget(label3);
        setLayout(vbox);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    LabelDemo w;
    w.show();
    return app.exec();
}
