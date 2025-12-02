#include "mptfloatplane.h"
#include "globalresource.h"

#include <QApplication>
#include <QHotkey>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    globalresource.init_db();

    mptfloatplane w;
    w.show();
    w.activateWindow();

    QHotkey hotkey(QKeySequence("Ctrl+Shift+Alt+Z"), true, &a); //The hotkey will be automatically registered
//    qDebug() << "Is registered:" << hotkey.isRegistered();

    QObject::connect(&hotkey, &QHotkey::activated, qApp, [&](){
        if (w.isVisible()) {
            w.hide();
        } else {
            w.show();
            w.activateWindow();
        }
    });

    return a.exec();
}
