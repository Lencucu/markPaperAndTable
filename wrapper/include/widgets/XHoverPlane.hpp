#ifndef XHOVERPLANE_HPP
#define XHOVERPLANE_HPP

class QHBoxLayout;
class QPropertyAnimation;

#include <QEvent>
#include <QTextBrowser>
#include <QScrollBar>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <QLineEdit>
#include <QGraphicsOpacityEffect>


class SearchBar : public QLineEdit {
    Q_OBJECT
public:
    using QLineEdit::QLineEdit;
    SearchBar(QWidget* parent = nullptr) : QLineEdit(parent) {
        connect(this, &SearchBar::committedTextChanged, this, [this](const QString &){
            // setStyleSheet("background-color: #D0E8D8;");
        });
    }

protected:
    void inputMethodEvent(QInputMethodEvent *event) override {
        // event->commitString() 是已经确认的文字
        if (!event->commitString().isEmpty()) {
            emit committedTextChanged(event->commitString());
        }
        QLineEdit::inputMethodEvent(event);
    }
    // 普通键盘输入（英文、数字、标点）
    void keyPressEvent(QKeyEvent *event) override {
        QLineEdit::keyPressEvent(event); // 先让 QLineEdit 处理输入
        emit committedTextChanged(text());
    }

signals:
    void committedTextChanged(const QString &text);
};


class XHoverPlane : public QWidget
{
    Q_OBJECT

public:
    XHoverPlane(QWidget *parent = nullptr);
    ~XHoverPlane();

    QGraphicsOpacityEffect* effect;
private:
    SearchBar* searchBar;
    QHBoxLayout* previews;
    QPropertyAnimation* anim;

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
