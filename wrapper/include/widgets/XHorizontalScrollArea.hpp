#ifndef XHORIZONTALSCROLLAREA_HPP
#define XHORIZONTALSCROLLAREA_HPP

#include <QScrollArea>
#include <QWheelEvent>
#include <QScrollBar>
#include <QPainter>
#include <QResizeEvent>

class GradientOverlay : public QWidget {
public:
    GradientOverlay(QWidget* parent = nullptr) : QWidget(parent) {
        setAttribute(Qt::WA_TransparentForMouseEvents);
        setAttribute(Qt::WA_NoSystemBackground);
    }

protected:
    void paintEvent(QPaintEvent*) override
    {
        QPainter p(this);
        QLinearGradient grad(0, 0, width(), 0);  // 水平渐变
        grad.setColorAt(0.01, QColor(0xD0, 0xE8, 0xD8, 255));   // 不透明
        grad.setColorAt(0.04, QColor(0, 0, 0, 0));     // 全透明
        grad.setColorAt(0.96, QColor(0, 0, 0, 0));     // 全透明
        grad.setColorAt(0.99, QColor(0xD0, 0xE8, 0xD8, 255));   // 不透明
        p.fillRect(rect(), grad);
    }
};

class XHorizontalScrollArea : public QScrollArea {
public:
    GradientOverlay *gradientoverlay = nullptr;
    XHorizontalScrollArea(QWidget* parent = nullptr) : QScrollArea(parent) {}

protected:
    void wheelEvent(QWheelEvent* event) override {
        // 获取滚轮滚动的角度
        int delta = event->angleDelta().y()*2/3; // 默认纵向滚动值
        // 用纵向滚轮值去滚动水平滚动条
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta);
        event->accept();
    }
    void resizeEvent(QResizeEvent *e) override {
        QSize size = e->size();
        size.setWidth(size.width()+2);
        if(gradientoverlay) gradientoverlay->resize(size);
        QScrollArea::resizeEvent(e);
    }
};


#endif // XHORIZONTALSCROLLAREA_HPP
