#include "mptfloatplane.h"
#include "globalresource.h"

#include <QApplication>
#include <QHotkey>
#include <QDebug>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>


int main(int argc, char *argv[])
{
    {
        LccToolbox::originstr str;
        str=("notes/design.md");
        LccToolbox::fileBox *what = new LccToolbox::fileBox{str.c_str()};
        delete what;
    }
    QApplication a(argc, argv);
    globalresource.query_db();

    // 托盘图标
    QSystemTrayIcon *tray = new QSystemTrayIcon(&a);
    tray->setIcon(QIcon("assets/mpt.png"));   // 换成你的图标
    tray->setToolTip("mpt");

    // 托盘菜单
    QMenu menu;
    QAction *quitAct = menu.addAction("退出");
    QObject::connect(quitAct, &QAction::triggered, &a, &QApplication::quit);

    tray->setContextMenu(&menu);
    tray->show();


    mptfloatplane w;
    w.show();
    w.setWindowOpacity(0.5);

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
