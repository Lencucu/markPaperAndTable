#ifndef MYWIDGET_HPP
#define MYWIDGET_HPP

#include <QWidget>
#include <QPixmap>
#include <QVector>
#include <QRandomGenerator>

class MyWidget : public QWidget {
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = nullptr);

public slots:
    void startDissolve();

protected:
    void paintEvent(QPaintEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    struct Particle {
        QPointF srcPos;      // 原始位置
        QPointF pos;         // 当前位置
        QPointF velocity;    // 速度
        float size;          // 当前大小
        float opacity;       // 透明度
        int life;            // 生命值（帧数）
        int maxLife;         // 最大生命值
    };

    QPixmap m_pixmap;
    QVector<Particle> m_particles;
    int m_timerId = 0;
    int m_frameCount = 0;
    float m_progress = 0.0f;   // 0~1 湮灭进度
    int m_particleCount = 0;   // 已生成的粒子数
};

#endif