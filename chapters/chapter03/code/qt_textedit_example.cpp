#include <QApplication>
#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

class TextEditDemo : public QWidget
{
public:
    TextEditDemo(QWidget *parent = nullptr) : QWidget(parent)
    {
        setWindowTitle("QTextEdit 示例");
        resize(400, 220);
        auto *label = new QLabel("请输入多行文本", this);
        label->setAlignment(Qt::AlignCenter);
        auto *textEdit = new QTextEdit(this);
        textEdit->setPlaceholderText("支持多行文本和富文本（HTML）...");
        auto *plainBtn = new QPushButton("显示纯文本", this);
        auto *htmlBtn = new QPushButton("显示富文本", this);
        connect(plainBtn, &QPushButton::clicked,
        this, [=](){
        label->setText(textEdit->toPlainText()); });
        connect(htmlBtn, &QPushButton::clicked,
        this, [=](){
        label->setText(textEdit->toHtml()); });
        auto *btnLayout = new QHBoxLayout;
        btnLayout->addWidget(plainBtn);
        btnLayout->addWidget(htmlBtn);
        auto *mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(textEdit);
        mainLayout->addLayout(btnLayout);
        mainLayout->addWidget(label);
        setLayout(mainLayout);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TextEditDemo w;
    w.show();
    return app.exec();
}
