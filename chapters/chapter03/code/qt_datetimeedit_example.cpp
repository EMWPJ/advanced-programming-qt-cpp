#include <QApplication>
#include <QWidget>
#include <QDateTimeEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class DateTimeEditDemo : public QWidget
{
public:
    DateTimeEditDemo(QWidget *parent = nullptr) : QWidget(parent)
    {
        setWindowTitle("QDateTimeEdit 示例");
        resize(400, 180);
        QLabel *label = new QLabel("请选择日期和时间", this);
        label->setAlignment(Qt::AlignCenter);
        QDateTimeEdit *dateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime(), this);
        dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
        dateTimeEdit->setCalendarPopup(true);
        QDateEdit *dateEdit = new QDateEdit(QDate::currentDate(), this);
        dateEdit->setDisplayFormat("yyyy-MM-dd");
        dateEdit->setCalendarPopup(true);
        QTimeEdit *timeEdit = new QTimeEdit(QTime::currentTime(), this);
        timeEdit->setDisplayFormat("HH:mm:ss");
        timeEdit->setCalendarPopup(true);
        // 信号槽连接
        connect(dateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, [=](const QDateTime &dt){
            label->setText("已选择：" + dt.toString("yyyy-MM-dd HH:mm:ss"));
        });
        connect(dateEdit, &QDateEdit::dateChanged, this, [=](const QDate &date){
            label->setText("已选择：" + date.toString("yyyy-MM-dd"));
        });
        connect(timeEdit, &QTimeEdit::timeChanged, this, [=](const QTime &time){
            label->setText("已选择：" + time.toString("HH:mm:ss"));
        });
        QHBoxLayout *hbox = new QHBoxLayout;
        hbox->addWidget(dateTimeEdit);
        hbox->addWidget(dateEdit);
        hbox->addWidget(timeEdit);
        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addLayout(hbox);
        vbox->addWidget(label, 1);
        setLayout(vbox);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    DateTimeEditDemo w;
    w.show();
    return app.exec();
}
