//#include <wrapper.hpp> // 模糊记忆：貌似是改向了global_uniforms，为了统一全局共享资源的貌似
#include <widgets/XHoverPlane.hpp>

#include <QApplication>
#include <QHotkey>
#include <QDebug>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>


int main(int argc, char *argv[])
{   QApplication a(argc, argv);
//    globalresource.query_sqlite_db();

    // 托盘图标
    QSystemTrayIcon *tray = new QSystemTrayIcon(&a);
    tray->setIcon(QIcon("logo.png"));   // 换成你的图标
    tray->setToolTip("mpt");

    // 托盘菜单
    QMenu menu;
    QAction *quitAct = menu.addAction("退出");
    QObject::connect(quitAct, &QAction::triggered, &a, &QApplication::quit);

    tray->setContextMenu(&menu);
    tray->show();


    XHoverPlane w;
    w.show();
    w.setWindowOpacity(0.05);

    QHotkey hotkey(QKeySequence("Ctrl+Shift+Alt+Z"), true, &a); //The hotkey will be automatically registered
//    qDebug() << "Is registered:" << hotkey.isRegistered();

    QObject::connect(&hotkey, &QHotkey::activated, qApp, [&](){
        if (w.isVisible()) {
            if(w.isActiveWindow())
                w.hide();
            else
                w.activateWindow();
        } else {
            w.show();
            w.activateWindow();
        }
    });

    return a.exec();
}
