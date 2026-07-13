#include "MyWidget.hpp"
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QMouseEvent>
#include <cmath>

MyWidget::MyWidget(QWidget *parent) : QWidget(parent), m_timerId(0) {
    resize(600, 450);
    setStyleSheet("background: #0a0a12;");
    
    // 点击窗口触发湮灭
}

void MyWidget::mousePressEvent(QMouseEvent *event) {
    if (m_timerId == 0) {
        startDissolve();
    }
    QWidget::mousePressEvent(event);
}

void MyWidget::startDissolve() {
    if (m_timerId != 0) return;
    
    // 生成窗口截图
    QPixmap temp(size());
    temp.fill(Qt::transparent);
    QPainter painter(&temp);
    
    // 绘制内容 - 请替换成你自己的内容
    painter.fillRect(rect(), QColor(20, 30, 50));
    
    painter.setPen(QPen(QColor(0, 255, 255), 3));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(rect().center(), 150, 150);
    painter.drawEllipse(rect().center(), 170, 170);
    painter.drawEllipse(rect().center(), 190, 190);
    
    painter.setPen(QPen(Qt::white, 2));
    painter.setFont(QFont("Arial", 40, QFont::Bold));
    painter.drawText(rect(), Qt::AlignCenter, "湮灭");
    
    painter.setFont(QFont("Arial", 16));
    painter.setPen(QColor(150, 200, 255));
    painter.drawText(rect().adjusted(0, 60, 0, 0), Qt::AlignHCenter | Qt::AlignTop, "点击窗口开始");
    
    // 装饰粒子
    for (int i = 0; i < 50; ++i) {
        int x = QRandomGenerator::global()->bounded(width());
        int y = QRandomGenerator::global()->bounded(height());
        painter.setPen(QColor(100, 200, 255, 50));
        painter.drawEllipse(x, y, 2, 2);
    }
    
    m_pixmap = temp;
    
    // 初始化粒子系统
    m_particles.clear();
    m_frameCount = 0;
    m_progress = 0.0f;
    m_particleCount = 0;
    
    // 从图片中采样像素作为粒子源
    QImage img = m_pixmap.toImage();
    int step = 3; // 采样步长，越小粒子越多
    
    for (int y = 0; y < img.height(); y += step) {
        for (int x = 0; x < img.width(); x += step) {
            QRgb color = img.pixel(x, y);
            int alpha = qAlpha(color);
            if (alpha > 50) { // 只取不透明的像素
                Particle p;
                p.srcPos = QPointF(x, y);
                p.pos = p.srcPos;
                p.size = step;
                p.opacity = 1.0f;
                // 每个粒子的生命周期不同，产生先后顺序
                p.maxLife = 50 + QRandomGenerator::global()->bounded(100);
                p.life = QRandomGenerator::global()->bounded(p.maxLife);
                
                // 初始速度为0
                p.velocity = QPointF(0, 0);
                m_particles.append(p);
            }
        }
    }
    
    qDebug() << "粒子数:" << m_particles.size();
    m_timerId = startTimer(16); // 60fps
}

void MyWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    if (m_timerId == 0 || m_particles.isEmpty()) {
        // 未开始特效，显示完整内容
        if (!m_pixmap.isNull()) {
            painter.drawPixmap(0, 0, m_pixmap);
        }
        return;
    }
    
    m_frameCount++;
    m_progress = qMin(1.0f, m_frameCount / 180.0f); // 3秒完成
    
    // 绘制黑色背景
    painter.fillRect(rect(), QColor(10, 10, 18));
    
    auto *rand = QRandomGenerator::global();
    bool allDead = true;
    
    // 进度影响：粒子速度越来越快，尺寸越来越小
    float speedMultiplier = 1.0f + m_progress * 8.0f;  // 速度逐渐加快
    float sizeShrink = 1.0f - m_progress * 0.7f;       // 尺寸逐渐缩小
    
    // 绘制所有粒子
    for (auto &p : m_particles) {
        // 如果粒子已死亡（透明度为0），跳过
        if (p.opacity <= 0) continue;
        allDead = false;
        
        // 生命周期到了，粒子开始飘散
        if (p.life < p.maxLife) {
            p.life++;
            // 生命值越高，飘散越剧烈
            float lifeRatio = p.life / (float)p.maxLife;
            
            // 随机加速度（风的效果）
            float windX = rand->bounded(2.0f) - 1.0f;
            float windY = rand->bounded(2.0f) - 1.0f;
            
            // 随着进度增加，加速度和速度都增大
            float accel = (0.5f + lifeRatio * 2.0f) * speedMultiplier;
            p.velocity += QPointF(windX * accel, windY * accel);
            
            // 限制最大速度
            float maxSpeed = 15.0f * speedMultiplier;
            float speed = sqrt(p.velocity.x() * p.velocity.x() + p.velocity.y() * p.velocity.y());
            if (speed > maxSpeed) {
                p.velocity = p.velocity / speed * maxSpeed;
            }
            
            // 更新位置
            p.pos += p.velocity;
            
            // 尺寸逐渐减小（模拟颗粒越来越小）
            float lifeFactor = 1.0f - lifeRatio * 0.5f;
            p.size = 3.0f * lifeFactor * sizeShrink;
            if (p.size < 0.5f) p.size = 0.5f;
            
            // 透明度逐渐降低
            p.opacity = 1.0f - lifeRatio * 1.2f;
            if (p.opacity < 0) p.opacity = 0;
            
        } else {
            // 生命周期结束，直接消失
            p.opacity = 0;
        }
    }
    
    // 绘制粒子
    for (const auto &p : m_particles) {
        if (p.opacity <= 0) continue;
        
        painter.setOpacity(p.opacity);
        
        // 从原图取色
        QColor color = m_pixmap.toImage().pixelColor(p.srcPos.toPoint());
        painter.setPen(QPen(color, p.size, Qt::SolidLine, Qt::RoundCap));
        painter.drawPoint(p.pos);
    }
    
    painter.setOpacity(1.0);
    
    // 如果所有粒子都消失了
    if (allDead && m_timerId != 0) {
        qDebug() << "湮灭完成";
        killTimer(m_timerId);
        m_timerId = 0;
        // 不关闭窗口，保持黑色
    }
}

void MyWidget::timerEvent(QTimerEvent *event) {
    if (event->timerId() == m_timerId) {
        update();
    }
}