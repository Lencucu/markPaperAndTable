#ifndef XHORIZONTALSCROLLAREA_HPP
#define XHORIZONTALSCROLLAREA_HPP

#include <QScrollArea>
#include <QWheelEvent>


class HorizontalScrollArea : public QScrollArea {
public:
    HorizontalScrollArea(QWidget* parent = nullptr) : QScrollArea(parent) {}

protected:
    void wheelEvent(QWheelEvent* event) override {
        // 获取滚轮滚动的角度
        int delta = event->angleDelta().y()*2/3; // 默认纵向滚动值
        // 用纵向滚轮值去滚动水平滚动条
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta);
        event->accept();
    }
};


#endif // XHORIZONTALSCROLLAREA_HPP
