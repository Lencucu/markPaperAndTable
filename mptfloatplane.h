#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>
#include <QResizeEvent>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QTextBrowser>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class preview : public QWidget{
    Q_OBJECT

public:
    preview(QWidget *parent = nullptr);
    ~preview();

private:
    QTextBrowser* textbrowser=nullptr;

protected:
    void resizeEvent(QResizeEvent *e){
        if(textbrowser) textbrowser->resize(e->size());
    }
};

class mptfloatplane : public QWidget
{
    Q_OBJECT

public:
    mptfloatplane(QWidget *parent = nullptr);
    ~mptfloatplane();

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
};
#endif // MAINWINDOW_H
