#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class mptfloatplane; }
QT_END_NAMESPACE

class mptfloatplane : public QWidget
{
    Q_OBJECT

public:
    mptfloatplane(QWidget *parent = nullptr);
    ~mptfloatplane();

private:
    Ui::mptfloatplane *ui;

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
