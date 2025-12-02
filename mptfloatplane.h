#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>
#include <list>

QT_BEGIN_NAMESPACE
namespace Ui {
    class mptfloatplane;
    class preview;
}
QT_END_NAMESPACE

class preview : public QWidget{
    Q_OBJECT

public:
    preview(QWidget *parent = nullptr);
    ~preview();

private:
    Ui::preview *ui;
};

class mptfloatplane : public QWidget
{
    Q_OBJECT

public:
    mptfloatplane(QWidget *parent = nullptr);
    ~mptfloatplane();

private:
    Ui::mptfloatplane *ui;
    std::list<QWidget*> preview;

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
