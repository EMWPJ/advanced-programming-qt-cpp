#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QConicalGradient>
#include <QPixmap>
#include <QFont>
#include <QPainterPath>

class PaintWidget : public QWidget {
protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        // 1. 设置抗锯齿和高质量渲染
painter.setRenderHint(QPainter::Antialiasing, true);
painter.setRenderHint(QPainter::TextAntialiasing, true);
painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        // 2. 绘制不同颜色和线宽的直线
        QPen pen1(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen1);
        painter.drawLine(20, 20, 180, 20);
        // 3. 绘制带虚线的矩形
        QPen pen2(Qt::blue, 2, Qt::DashLine);
        painter.setPen(pen2);
        painter.drawRect(20, 40, 80, 60);
        // 4. 绘制填充线性渐变的椭圆
        QLinearGradient linearGrad(60, 120, 140, 180);
        linearGrad.setColorAt(0, Qt::yellow);
        linearGrad.setColorAt(1, Qt::green);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(linearGrad));
        painter.drawEllipse(100, 40, 80, 60);
        // 5. 绘制填充径向渐变的圆（带阴影效果）
        QRadialGradient radialGrad(60, 160, 30, 60, 160);
        radialGrad.setColorAt(0, Qt::white);
        radialGrad.setColorAt(1, Qt::darkGray);
        painter.setBrush(QBrush(radialGrad));
        painter.drawEllipse(40, 140, 40, 40);
        // 6. 绘制锥形渐变的扇形
        QConicalGradient conicalGrad(180, 180, 0);
        conicalGrad.setColorAt(0, Qt::red);
        conicalGrad.setColorAt(0.5, Qt::yellow);
        conicalGrad.setColorAt(1, Qt::red);
        painter.setBrush(QBrush(conicalGrad));
        painter.setPen(QPen(Qt::darkRed, 2));
        QRectF arcRect(160, 160, 40, 40);
        painter.drawPie(arcRect, 30 * 16, 120 * 16);
        // 7. 绘制多边形
        painter.setPen(QPen(Qt::darkMagenta, 2));
        painter.setBrush(QBrush(Qt::cyan, Qt::Dense4Pattern));
        QPoint points[4] = {QPoint(120, 120), QPoint(180, 120), QPoint(170, 180), QPoint(130, 180)};
        painter.drawPolygon(points, 4);
        // 8. 绘制贝塞尔曲线
        painter.setPen(QPen(Qt::darkBlue, 2, Qt::DashDotLine));
        QPainterPath path;
        path.moveTo(20, 120);
        path.cubicTo(60, 80, 100, 160, 180, 100);
        painter.drawPath(path);
        // 9. 绘制带背景色的文本
        painter.setPen(Qt::black);
        painter.setFont(QFont("微软雅黑", 14, QFont::Bold));
        painter.setBrush(QBrush(Qt::lightGray));
        painter.drawRect(20, 200, 160, 30);
        painter.drawText(25, 222, "QPainter功能演示");
        // 10. 绘制图片（假设有一张图片在同目录下）
        QPixmap pix("qt_logo.png");
        if (!pix.isNull())
            painter.drawPixmap(120, 10, 60, 60, pix);
        // 11. 保存和恢复状态
        painter.save();
        painter.translate(100, 100);
        painter.rotate(30);
        painter.setPen(QPen(Qt::darkGreen, 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(-20, -20, 40, 40);
        painter.restore();
        // 12. 剪切区域
        painter.setClipRect(150, 150, 40, 40);
        painter.setBrush(Qt::red);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(140, 140, 60, 60);
        // 13. 绘制透明图形
        QColor semiTransBlue(0, 0, 255, 100);
        painter.setBrush(QBrush(semiTransBlue));
        painter.setPen(Qt::NoPen);
        painter.drawRect(60, 60, 60, 60);
        // 14. 绘制点和椭圆弧
        painter.setPen(QPen(Qt::darkCyan, 3));
        painter.drawPoint(200, 200);
        painter.setPen(QPen(Qt::darkYellow, 2));
        painter.drawArc(30, 180, 60, 40, 30 * 16, 120 * 16);
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    PaintWidget widget;
    widget.resize(240, 260);
    widget.setWindowTitle("QPainter用法详解示例");
    widget.show();
    return app.exec();
}
