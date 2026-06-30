#ifndef XHORIZONTALSCROLLAREA_HPP
#define XHORIZONTALSCROLLAREA_HPP

class QGraphicsOpacityEffect;
class QPropertyAnimation;
class XHorizontalScrollArea;
class XHoverPlane;

#include <QScrollArea>
#include <QWheelEvent>
#include <QScrollBar>
#include <QPainter>
#include <QResizeEvent>
#include <QPropertyAnimation>

extern bool state_float_is_active;
extern bool state_main_is_active;
extern XHorizontalScrollArea* float_area;
extern XHoverPlane* main_area;








class GradientOverlay : public QWidget {
public:
    double radio = 0.04;
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
        grad.setColorAt(radio, QColor(0, 0, 0, 0));     // 全透明
        grad.setColorAt(1-radio, QColor(0, 0, 0, 0));     // 全透明
        grad.setColorAt(0.99, QColor(0xD0, 0xE8, 0xD8, 255));   // 不透明
        p.fillRect(rect(), grad);
    }
};




class XHorizontalScrollArea : public QScrollArea {
    Q_OBJECT
signals:
    void ready2deactivate();
    void try2activate();

public:
    GradientOverlay *gradientoverlay = nullptr;
    QGraphicsOpacityEffect* effect_float = nullptr;
    QPropertyAnimation* anim_float = nullptr;
    bool is_float = false;
    XHorizontalScrollArea(QWidget* parent = nullptr);
    void connect4float();

protected:
    void wheelEvent(QWheelEvent* event) override {
        // 获取滚轮滚动的角度
        int delta = event->angleDelta().y()*2/3; // 默认纵向滚动值
        // 用纵向滚轮值去滚动水平滚动条
        QPropertyAnimation* anim = new QPropertyAnimation(horizontalScrollBar(), "value");

        anim->setDuration(300);
        anim->setStartValue(horizontalScrollBar()->value());
        anim->setEndValue(horizontalScrollBar()->value() - delta);   // 目标滚动位置
        anim->setEasingCurve(QEasingCurve::OutCubic);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
        event->accept();
    }
    void resizeEvent(QResizeEvent *e) override {
        QSize size = e->size();
        size.setWidth(size.width()+2);
        resizeOverlay(size);
        QScrollArea::resizeEvent(e);
    }
    bool event(QEvent *e) override;
public:
    void show_diy();
    void hide_diy();
    void resizeOverlay(QSize size){
        if(gradientoverlay) gradientoverlay->resize(size);
    }
};








#endif // XHORIZONTALSCROLLAREA_HPP