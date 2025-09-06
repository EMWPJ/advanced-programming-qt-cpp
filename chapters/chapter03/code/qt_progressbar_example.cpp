#include <QApplication>
#include <QWidget>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTimer>
#include <QLabel>

class ProgressBarDemo : public QWidget {
public:
    ProgressBarDemo(QWidget *parent = nullptr) : QWidget(parent), value(0) {
setWindowTitle("QProgressBar 示例");
resize(400, 180);
auto *label = new QLabel("点击“开始”模拟进度", this);
label->setAlignment(Qt::AlignCenter);
auto *progressBar = new QProgressBar(this);
progressBar->setRange(0, 100);
auto *startBtn = new QPushButton("开始", this);
auto *resetBtn = new QPushButton("重置", this);
auto *timer = new QTimer(this);
timer->setInterval(50);
connect(startBtn, &QPushButton::clicked, this, [=]() {
    if (!timer->isActive() && value < 100)
            timer->start(); });
connect(resetBtn, &QPushButton::clicked, this, [=]() {
    timer->stop();
    value = 0;
    progressBar->reset();
    label->setText("点击“开始”模拟进度"); });
connect(timer, &QTimer::timeout, this, [=]() {
    if (value < 100) {
        progressBar->setValue(++value);
        label->setText(QString("当前进度：%1%").arg(value));
    } else {
        timer->stop();
        label->setText("进度完成！");
    } });
auto *layout = new QVBoxLayout(this);
layout->addWidget(label);
layout->addWidget(progressBar);
layout->addWidget(startBtn);
layout->addWidget(resetBtn);
setLayout(layout);
}

private:
    int value;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ProgressBarDemo w;
    w.show();
    return app.exec();
}
