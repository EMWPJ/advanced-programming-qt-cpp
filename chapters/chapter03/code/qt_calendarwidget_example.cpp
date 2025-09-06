#include <QApplication>
#include <QWidget>
#include <QCalendarWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QDate>

class CalendarWidgetDemo : public QWidget
{
public:
    CalendarWidgetDemo(QWidget *parent = nullptr) : QWidget(parent)
    {
        setWindowTitle("QCalendarWidget 示例");
        resize(400, 250);

        QLabel *label = new QLabel("请选择日期", this);
        label->setAlignment(Qt::AlignCenter);

        QCalendarWidget *calendar = new QCalendarWidget(this);
        calendar->setGridVisible(true);
        calendar->setFirstDayOfWeek(Qt::Monday);
        calendar->setSelectedDate(QDate::currentDate());

        // 信号槽连接
        connect(calendar, &QCalendarWidget::selectionChanged,
        this, [=](){
        QDate date = calendar->selectedDate();
        label->setText("已选择：" + date.toString("yyyy-MM-dd"));
        });

        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addWidget(calendar);
        vbox->addWidget(label, 1);
        setLayout(vbox);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    CalendarWidgetDemo w;
    w.show();
    return app.exec();
}
