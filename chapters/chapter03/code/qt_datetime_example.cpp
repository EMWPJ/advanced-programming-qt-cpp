#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QTimeZone>
#include <QDebug>
#include <QString>
#include <QLocale>
#include <QElapsedTimer>
#include <iostream>
void dateTimeExample()
{
    // 1. 获取当前日期和时间（本地和UTC）
    QDateTime current = QDateTime::currentDateTime();
    QDateTime currentUtc = QDateTime::currentDateTimeUtc();
    qDebug() << "当前本地日期和时间:" << current.toString("yyyy-MM-dd hh:mm:ss");
    qDebug() << "当前UTC日期和时间:" << currentUtc.toString("yyyy-MM-dd hh:mm:ss");
    // 2. 获取当前日期和时间的详细信息
    QDate date = current.date();
    QTime time = current.time();
    qDebug() << "当前日期:" << date.toString("yyyy-MM-dd");
    qDebug() << "当前时间:" << time.toString("hh:mm:ss");
    qDebug() << "当前星期:" << QLocale::system().dayName(date.dayOfWeek());
    qDebug() << "今年的第几天:" << date.dayOfYear();
    qDebug() << "今年的第几周:" << date.weekNumber();
    // 3. 构造指定日期和时间
    QDateTime dt(QDate(2024, 6, 1), QTime(12, 30, 0));
    qDebug() << "指定日期和时间:" << dt.toString("yyyy-MM-dd hh:mm:ss");
    // 4. 日期时间加减
    QDateTime future = current.addDays(5).addSecs(3600);
    QDateTime past = current.addYears(-1).addMonths(-2);
    qDebug() << "5天后加1小时:" << future.toString("yyyy-MM-dd hh:mm:ss");
    qDebug() << "1年前2个月前:" << past.toString("yyyy-MM-dd hh:mm:ss");
    // 5. 时间戳转换
    qint64 timestamp = current.toSecsSinceEpoch();
    qint64 msecTimestamp = current.toMSecsSinceEpoch();
    qDebug() << "当前时间戳(秒):" << timestamp;
    qDebug() << "当前时间戳(毫秒):" << msecTimestamp;
    QDateTime fromTimestamp = QDateTime::fromSecsSinceEpoch(timestamp);
    QDateTime fromMsecTimestamp = QDateTime::fromMSecsSinceEpoch(msecTimestamp);
    qDebug() << "由时间戳(秒)还原:" << fromTimestamp.toString("yyyy-MM-dd hh:mm:ss");
    qDebug() << "由时间戳(毫秒)还原:" << fromMsecTimestamp.toString("yyyy-MM-dd hh:mm:ss");
    // 6. 字符串与QDateTime互转
    QString dateTimeStr = "2024-06-01 15:20:30";
    QDateTime parsed = QDateTime::fromString(dateTimeStr, "yyyy-MM-dd hh:mm:ss");
    qDebug() << "字符串转QDateTime:" << parsed.toString("yyyy-MM-dd hh:mm:ss");
    QString formatted = parsed.toString(Qt::ISODate);
    qDebug() << "QDateTime转ISO字符串:" << formatted;
    // 7. 时区转换
    QDateTime utc = current.toUTC();
    qDebug() << "UTC时间:" << utc.toString("yyyy-MM-dd hh:mm:ss");
    QTimeZone beijingZone("Asia/Shanghai");
    if (beijingZone.isValid()) {
        QDateTime beijing = utc.toTimeZone(beijingZone);
        qDebug() << "北京时间:" << beijing.toString("yyyy-MM-dd hh:mm:ss");
    } else {
        qDebug() << "无法识别Asia/Shanghai时区";
    }
    // 8. 日期时间计算
    QDateTime nextMonth = current.addMonths(1);
    QDateTime nextYear = current.addYears(1);
    qDebug() << "下个月:" << nextMonth.toString("yyyy-MM-dd hh:mm:ss");
    qDebug() << "明年:" << nextYear.toString("yyyy-MM-dd hh:mm:ss");
    // 9. 日期时间比较
    if (current > dt)
        qDebug() << "当前日期和时间大于指定日期时间";
    else if (current < dt)
        qDebug() << "当前日期和时间小于指定日期时间";
    else
        qDebug() << "当前日期和时间等于指定日期时间";
    // 10. 日期时间间隔（天、秒、毫秒）
    int daysDiff = current.daysTo(future);
    qint64 secsDiff = current.secsTo(future);
    qint64 msecsDiff = current.msecsTo(future);
    qDebug() << "当前到未来的天数差:" << daysDiff;
    qDebug() << "当前到未来的秒数差:" << secsDiff;
    qDebug() << "当前到未来的毫秒数差:" << msecsDiff;
    // 11. QDate、QTime常用操作
    QDate leapDate(2024, 2, 29);
    qDebug() << "2024年2月29日是否为闰年:" << leapDate.isLeapYear(leapDate.year());
    QTime t1(10, 20, 30);
    QTime t2 = t1.addSecs(3600);
    qDebug() << "10:20:30加1小时:" << t2.toString("hh:mm:ss");
    qDebug() << "两个时间的秒数差:" << t1.secsTo(t2);
    // 12. QDateTime的有效性与空值判断
    QDateTime invalid;
    qDebug() << "未初始化QDateTime是否有效:" << invalid.isValid();
    QDateTime nullDt = QDateTime();
    qDebug() << "空QDateTime是否为null:" << nullDt.isNull();
    // 13. 计时器
    qDebug() << "计时开始";
    QElapsedTimer timer;
    timer.start();
    for (int i = 0; i < 1000000; i++) {
        QDateTime::currentDateTime();
    }
    qDebug() << "计时结束，耗时(毫秒):" << timer.elapsed();
}

int main()
{
    dateTimeExample();
    std::cin.get();
    return 0;
}
