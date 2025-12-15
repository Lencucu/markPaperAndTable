#ifndef XHOVERPLANE_HPP
#define XHOVERPLANE_HPP

class QLineEdit;
class QHBoxLayout;

#include <QEvent>
#include <QTextBrowser>
#include <QScrollBar>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>


class HoverPlane : public QWidget
{
    Q_OBJECT

public:
    HoverPlane(QWidget *parent = nullptr);
    ~HoverPlane();

private:
    QLineEdit* searchBar;
    QHBoxLayout* previews;

protected:
    bool event(QEvent *e) override {
        if (e->type() == QEvent::WindowDeactivate) {
            qDebug() << "窗口及子控件失去焦点，脱离激活";
            setWindowOpacity(0.5);
        }
        if (e->type() == QEvent::WindowActivate) {
            qDebug() << "窗口获得焦点";
            setWindowOpacity(1);
        }
        return QWidget::event(e);
    }

    void paintEvent(QPaintEvent*) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // 设置圆角背景颜色
        QColor bgColor(0xD0,0xE8,0xD8); // #3498db
        painter.setBrush(bgColor);
        painter.setPen(Qt::NoPen);

        // 绘制圆角矩形
        painter.drawRoundedRect(rect(), 5, 7);
    }
};
#endif // XHOVERPLANE_HPP
