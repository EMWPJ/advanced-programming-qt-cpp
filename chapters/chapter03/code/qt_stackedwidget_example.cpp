#include <QApplication>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget window;
    window.setWindowTitle("QStackedWidget 示例");
    window.resize(400, 200);

    QStackedWidget *stack = new QStackedWidget(&window);
    stack->addWidget(new QLabel("这是第一个页面"));
    stack->addWidget(new QLabel("这是第二个页面"));

    QPushButton *prev = new QPushButton("上一页");
    QPushButton *next = new QPushButton("下一页");

    QObject::connect(prev, &QPushButton::clicked, [&](){
        int idx = stack->currentIndex();
        if (idx > 0) stack->setCurrentIndex(idx - 1);
    });
    QObject::connect(next, &QPushButton::clicked, [&](){
        int idx = stack->currentIndex();
        if (idx < stack->count() - 1) stack->setCurrentIndex(idx + 1);
    });

    QVBoxLayout *layout = new QVBoxLayout(&window);
    layout->addWidget(stack);
    layout->addStretch();
    layout->addWidget(prev);
    layout->addWidget(next);
    window.show();
    return app.exec();
}
