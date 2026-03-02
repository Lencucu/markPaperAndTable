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


class XHoverPlane : public QWidget
{
    Q_OBJECT

public:
    XHoverPlane(QWidget *parent = nullptr);
    ~XHoverPlane();

private:
    QLineEdit* searchBar;
    QHBoxLayout* previews;

protected:
    bool event(QEvent *e) override;

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
